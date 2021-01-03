#include "common.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fmt/core.h>
#include <string_view>
#include <vector>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <ranges>
#include <execution>
#include <iterator>
namespace ranges = std::ranges;

bool dfs(std::vector<int> rule, std::unordered_map<int, std::vector<std::vector<int> > > &rule_map, std::unordered_map<int, char> &leaf_map,
		std::string_view message) {
	auto i { 0U };
	for (auto r : rule) {
		if (leaf_map.contains(r)) {
			if (not message.empty() and leaf_map[r] == message[0]) {
				message = message.substr(1);
			} else {
				return false;
			}
		} else {
			if (rule_map[r].size() == 1U) {
				std::vector<int> new_rule = rule_map[r][0];
				new_rule.insert(new_rule.end(), rule.begin() + i+1, rule.end());
				return dfs(new_rule, rule_map, leaf_map, message);
			} else {
				std::vector<int> new_rule = rule_map[r][0];
				new_rule.insert(new_rule.end(), rule.begin() + i+1, rule.end());
				auto new_rule2 = rule_map[r][1];
				new_rule2.insert(new_rule2.end(), rule.begin() + i+1, rule.end());
				return dfs(new_rule, rule_map, leaf_map, message) or dfs(new_rule2, rule_map, leaf_map, message);
			}
		}
		i++;
	}
	return message.size() == 0U;
}

int insert_child_and_clear(auto &inverse_map, auto &instant_replace_map, int key, std::vector<int> &child) {
	if (child.size() == 1) {
		instant_replace_map.insert( { child[0], key });
	} else if (child.size() == 2) {
		Point p { child[0], child[1] };
		if (not inverse_map.contains(p)) {
			inverse_map.insert( { p, std::unordered_set<int> { key } });
		} else {
			inverse_map.at(p).insert(key);

		}
	}
	child.clear();
	return key;
}

auto parse_input(auto const &lines) {
	std::stringstream ss { };
	std::unordered_map<Point, std::unordered_set<int>> inverse_map;
	std::unordered_map<int, std::vector<std::vector<int> > > rule_map;
	std::unordered_map<int, char> leaf_map;
	std::unordered_map<char, int> literal_map;
	std::unordered_map<int, int> instant_replace_map;
	std::vector<std::string> messages;
	bool parse_rules { true };
	for (auto const &line : lines) {
		if (line.empty()) {
			parse_rules = false;
			continue;
		}
		if (parse_rules) {
			std::string token;
			std::vector<std::vector<int> > sub_rules;
			std::vector<int> child;
			int key;
			ss.clear();
			ss << line;
			ss >> token;
			token.erase(token.size() - 1);
			key = std::stoi(token);
			//fmt::print("key {}\n", key);
			while (ss >> token) {
				if (token[0] == '"') {
					leaf_map[key] = token[1];
					literal_map.insert( { token[1], key });
					child.push_back(-1);
				} else if (token == "|") {
					sub_rules.push_back(child);
					//	fmt::print("key {} child_size {}\n", key, child.size());
					insert_child_and_clear(inverse_map, instant_replace_map, key, child);
				} else {
					child.push_back(std::stoi(token));
				}
			}
			sub_rules.push_back(child);

			//fmt::print("key {} child_size {}\n", key, child.size());
			rule_map.insert( { key, sub_rules });
			insert_child_and_clear(inverse_map, instant_replace_map, key, child);

		} else {
			messages.push_back(line);
		}
	}
	return std::make_tuple(rule_map, leaf_map, inverse_map, instant_replace_map, literal_map, messages);

}
auto cartesian_product(std::unordered_set<int> const &a, std::unordered_set<int> const &b) {
	std::vector<Point> points { };
	points.reserve(a.size() * b.size());
	ranges::for_each(a, [&](int A) {
		ranges::for_each(b, [A, &points](int B) {
			points.push_back(Point { A, B });
			//fmt::print("point is {},{}\n", A, B);
		}
		);
	});
	return points;
}

bool cyk(std::unordered_map<Point, std::unordered_set<int>> const &inverse_map, std::unordered_map<char, int> const &literal_map,
		std::unordered_map<int, int> const &instant_replace_map, std::string_view message) {
	auto const size { message.size() };
	std::vector<std::vector<std::unordered_set<int>>> table;
	table.resize(size);
	table[0].reserve(size);
	ranges::transform(message, std::back_inserter(table[0]), [&literal_map](char const c) {
		return std::unordered_set<int> { literal_map.at(c) };
	});
	ranges::for_each(table[0], [&instant_replace_map](auto &s) {
		if (instant_replace_map.contains(*s.begin())) {
			s.insert(instant_replace_map.at(*s.begin()));
		}
	});
	for (auto l { 2U }; l <= size; l++) {
		table[l - 1].resize(size - l + 1);

		for (auto s { 0U }; s <= size - l; s++) {
			for (auto p { 1U }; p < l; p++) {
				auto const points = cartesian_product(table.at(p - 1).at(s), table.at(l - p - 1).at(s + p));
				for (auto const &p : points) {
					if (inverse_map.contains(p)) {
						std::unordered_set<int> to_insert = inverse_map.at(p);
						table.at(l - 1).at(s).insert(to_insert.begin(), to_insert.end());
						for (auto const &instant : to_insert) {
							if (instant_replace_map.contains(instant)) {
								table.at(l - 1).at(s).insert(instant_replace_map.at(instant));
							}
						}
					}
				}

			}
		}
	}
	return table[size - 1][0].contains(0);

}

int main() {
	constexpr auto file_name = "build/input/input_19.txt";
	auto const lines = AOC::parse_lines(file_name);
	std::unordered_map<int, std::vector<std::vector<int> > > rule_map;
	std::unordered_map<int, char> leaf_map;
	std::vector<std::string> messages;
	std::unordered_map<Point, std::unordered_set<int>> inverse_map;
	std::unordered_map<char, int> literal_map;
	std::unordered_map<int, int> instant_replace_map;
	std::tie(rule_map, leaf_map, inverse_map, instant_replace_map, literal_map, messages) = parse_input(lines);
	std::vector<bool> matched { };
	matched.resize(messages.size());
	std::transform(std::execution::par_unseq, messages.begin(), messages.end(), matched.begin(),
	//[&inverse_map, &literal_map, &instant_replace_map](auto const &mess) {
	//return cyk(inverse_map, literal_map, instant_replace_map, mess);
			[&rule_map, &leaf_map](auto  &mess) {
				return dfs(rule_map[0][0], rule_map, leaf_map, mess);
			}
	);
	fmt::print("Part 1: {}\n", std::count(std::execution::par_unseq, matched.begin(), matched.end(), true));
	rule_map[8].push_back({42, 8});
	rule_map[11].push_back({42, 666});
	rule_map[666].push_back({11, 31});
	inverse_map.insert( { Point { 42, 8 }, { 8 } });
	inverse_map.insert( { Point { 11, 31 }, { 666 } });
	inverse_map.insert( { Point { 42, 666 }, { 11 } });
	std::transform(std::execution::par_unseq, messages.begin(), messages.end(), matched.begin(), matched.begin(),
			//[&inverse_map, &literal_map, &instant_replace_map](auto const &mess, bool matched) {
			//	return matched or cyk(inverse_map, literal_map, instant_replace_map, mess);
			[&rule_map, &leaf_map](auto  &mess, bool matched) {
							return matched or dfs(rule_map[0][0], rule_map, leaf_map, mess);

			});
	fmt::print("Part 2: {}\n", std::count(std::execution::par_unseq, matched.begin(), matched.end(), true)); //294
	return EXIT_SUCCESS;
}

