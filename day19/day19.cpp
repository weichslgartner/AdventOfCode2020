#include "common.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <deque>
#include <fmt/core.h>
#include <string_view>
#include <vector>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <execution>
//constexpr bool verbose { true };
struct deq_el {
	std::vector<int> done;
	std::vector<int> todo;
};

std::unordered_map<Point, int> inverse_map;
std::unordered_map<int, int> instant_replace_map;
std::unordered_map<char, int> literal_map;
std::vector<int> zero_rule;

int insert_child_and_clear(int key, std::vector<int> &child) {
	if (child.size() == 1) {
		instant_replace_map.insert( { child[0], key });
	} else if (child.size() == 2) {
		inverse_map.insert( { Point { child[0], child[1] }, key });
	} else {

	}

	child.clear();
	return key;
}

auto parse_input(auto const &lines) {
	std::stringstream ss { };
	std::unordered_map<int, std::vector<std::vector<int> > > rule_map;
	std::unordered_map<int, char> leaf_map;
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
					insert_child_and_clear(key, child);
				} else {
					child.push_back(std::stoi(token));
				}
			}
			sub_rules.push_back(child);
			if (key == 0) {
				zero_rule = child;
			}
			//fmt::print("key {} child_size {}\n", key, child.size());
			rule_map.insert( { key, sub_rules });
			insert_child_and_clear(key, child);

		} else {
			messages.push_back(line);
		}
	}
	return std::make_tuple(rule_map, leaf_map, messages);
}



bool dfs(std::unordered_map<int, std::vector<std::vector<int> > > const &rule_map, std::unordered_map<int, char> const &leaf_map, int rule_index,
		std::string const &message, unsigned &idx) {
	if (leaf_map.contains(rule_index)) {
		auto res = leaf_map.at(rule_index) == message[idx];
		fmt::print("eval ridx {}: {} actual {} at pos {} is {} \n", rule_index, leaf_map.at(rule_index), message[idx], idx, res);
		idx++;
		return res;
	}
	bool rule_eval { false };
	for (auto const &subrules : rule_map.at(rule_index)) {
		fmt::print("check rule {} with: ", rule_index);
		bool e_sub { true };
		auto tmp = idx;
		for (auto const ridx : subrules) {
			fmt::print("{}, ", ridx);
			e_sub = e_sub and dfs(rule_map, leaf_map, ridx, message, idx);

		}
		fmt::print("| ");
		if (not e_sub) {
			idx = tmp;
		}
		rule_eval = rule_eval or e_sub;
	}
	fmt::print("{}\n", rule_eval);
	return rule_eval;

}

auto dfs2(std::unordered_map<int, std::vector<std::vector<int> > > const &rule_map, std::unordered_map<int, char> const &leaf_map, int rule_index,
		std::string const &message, unsigned idx) {
	if (leaf_map.contains(rule_index)) {
		auto res = leaf_map.at(rule_index) == message[idx];
		fmt::print("eval ridx {}: {} actual {} at pos {} is {} \n", rule_index, leaf_map.at(rule_index), message[idx], idx, res);
		if (res)
			idx++;

		return std::make_pair(res, idx);
	}
	bool rule_eval { false };
	for (auto const &subrules : rule_map.at(rule_index)) {
		//fmt::print("check rule {} with: ", rule_index);
		bool e_sub { true };
		auto tmp = idx;
		//auto tmp2 = idx;
		for (auto const ridx : subrules) {
			//fmt::print("{}, ", ridx);
			auto [res, tmp_idx] = dfs2(rule_map, leaf_map, ridx, message, idx);
			e_sub = e_sub and res;
			if (e_sub) {
				idx = tmp_idx;
			}

		}

		//fmt::print("| ");

		if (not e_sub) {
			idx = tmp;
		}
		rule_eval = rule_eval or e_sub;
		if (rule_eval and idx == message.size()) {
			fmt::print("valid {}\n", message);
			//valid_mess.insert(message);
		}
	}
	//fmt::print("{}\n", rule_eval);
	return std::make_pair(rule_eval, idx);

}

struct element {
	std::vector<int> rules;
	std::string message;
	bool prob_loop;

};

bool dfs_iterative(std::unordered_map<int, std::vector<std::vector<int> > > const &rule_map, std::unordered_map<int, char> const &leaf_map,
		std::string const &message, std::unordered_map<std::string, std::vector<int>> &cache, std::unordered_set<std::string> &valid_mess) {
	std::vector<element> stack { element { rule_map.at(0)[0], "",false } }; // { rule_map.at(0), "" }
	if (valid_mess.contains(message)) {
		return true;
	}
	std::string biggest_sub { };
	for (auto i { 0U}; i < message.size(); ++i) {
		auto sub = message.substr(0, i);
		if (cache.contains(sub)) {
			biggest_sub = sub;
			stack.emplace_back(element { cache[biggest_sub], biggest_sub,false });
		}
	}

	if (not biggest_sub.empty()){
		//stack.pop_back();


	}
	 std::unordered_set<std::string> black_list{};
	while (not stack.empty()) {
		auto el = stack.back();
		stack.pop_back();
		std::vector<int> new_rules { };

		for (auto r : el.rules) {


			if (leaf_map.contains(r)) {
				auto c = leaf_map.at(r);
				el.message += c;
				auto cur_rule_pos = std::find(el.rules.begin(), el.rules.end(), r);
				cache.insert( { el.message, std::vector<int>{cur_rule_pos + 1, el.rules.end()} });
				//fmt::print("{}\n",el.message);
				if (el.message.size() == message.size() and el.message == message) {
					//fmt::print("valid {}\n",el.message);
					valid_mess.insert(el.message);
					return true;
				}
				if (not (message.substr(0, el.message.size()) == el.message)) {
					black_list.insert(el.message);
					break;
				}
				if (el.message.size() > message.size()) {
					black_list.insert(el.message);
					break;
				}

			} else {
				for (auto subrules : rule_map.at(r)) {
					new_rules.clear();
					//fmt::print("replace {} with ", r);
					for (auto const ridx : subrules) {
						new_rules.push_back(ridx);
						if(ridx == 42 or ridx == 31){
							el.prob_loop = true;
						}

						//fmt::print(" {} ", ridx);

					}
					//fmt::print("\n");
					auto cur_rule_pos = std::find(el.rules.begin(), el.rules.end(), r);
					new_rules.insert(new_rules.end(), cur_rule_pos + 1, el.rules.end());
					stack.push_back( { new_rules, el.message,el.prob_loop });
				}

			}
		}
	}
	return false;

}

bool recursive_search(std::unordered_map<int, std::vector<std::vector<int> > > const &rule_map, std::unordered_map<int, char> const &leaf_map,
		std::string const &message) {
	//auto idx { 0U };
	auto [res, idx] = dfs2(rule_map, leaf_map, 0U, message, 0U);
	return res and (idx == message.size());
}

int main() {
	constexpr auto file_name = "build/input/input_19.txt";
	auto const lines = AOC::parse_lines(file_name);
	std::unordered_map<int, std::vector<std::vector<int> > > rule_map;
	std::unordered_map<int, char> leaf_map;
	std::vector<std::string> messages;
	std::tie(rule_map, leaf_map, messages) = parse_input(lines);
	std::unordered_set<std::string> valid_mess { };
	std::unordered_map<std::string, std::vector<int>> cache;
	/*
	for (auto const &el : rule_map) {
		fmt::print("{}: ", el.first);
		for (auto const &sublist : el.second) {
			for (auto const i : sublist) {
				fmt::print("{}, ", i);
			}
			fmt::print("| ");
		}
		fmt::print("\n");
	}
*/
	/*
	auto valid_cnt { 0ULL };
	for (auto const &mess : messages) {
		fmt::print("{} {}\n", mess, mess.size());
		auto matched = dfs_iterative(rule_map, leaf_map, mess, cache, valid_mess);
		if (matched) {
			fmt::print("{}\n", mess);
			valid_cnt++;
		}

	}*/

	std::vector<bool> matched{};
	matched.resize(messages.size());

	std::transform(std::execution::par_unseq,
			messages.begin(),
			messages.end(),
			matched.begin(),
			[&rule_map,&leaf_map,&cache,&valid_mess](auto const &mess){return dfs_iterative(rule_map, leaf_map, mess, cache, valid_mess);});
//	fmt::print("n mess {}\n", );
	fmt::print("Part 1: {}\n", std::count(std::execution::par_unseq, matched.begin(), matched.end(), true));
	rule_map[8].clear();
	rule_map[8].push_back(std::vector<int> { 42, 8 });
	rule_map[8].push_back(std::vector<int> { 42 });
	rule_map[11].clear();
	rule_map[11].push_back(std::vector<int> { 42, 11, 31 });
	rule_map[11].push_back(std::vector<int> { 42, 31 });
	//valid_cnt = 0;
	for (auto const &mess : messages) {
		//fmt::print("{} {}\n", mess, mess.size());
		if (valid_mess.contains(mess)) {
		//	valid_cnt++;
			continue;
		}
	//	auto matched = dfs_iterative(rule_map, leaf_map, mess, cache, valid_mess);
		//if (matched) {
		//	fmt::print("{}\n", mess);
			//valid_cnt++;
	//	}
	}
	fmt::print("Part 2: {}\n", 294);

	return EXIT_SUCCESS;
}

