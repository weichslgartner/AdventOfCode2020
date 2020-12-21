#include "common.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <deque>
#include <fmt/core.h>
#include <vector>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
namespace ranges = std::ranges;

auto parse_input(std::vector<std::string> const &lines) {
	std::stringstream ss { };
	std::string token { };
	std::unordered_map<std::string, std::unordered_set<std::string> > allergense2ingrediens { };
	std::vector<std::string> ingredient_buffer { };
	std::vector<std::string> all_ingredients { };
	std::unordered_set<std::string> allergenes { };
	for (auto const &line : lines) {
		bool is_ingredient { true };
		ingredient_buffer.clear();
		allergenes.clear();
		ss.clear();
		ss << line;
		while (ss >> token) {
			if (token.find("contains") != std::string::npos) {
				is_ingredient = false;
				continue;
			}
			if (is_ingredient) {
				ingredient_buffer.push_back(token);
				all_ingredients.push_back(token);
			} else {
				allergenes.insert(token.substr(0, token.size() - 1));
			}
		}
		for (auto const &allergene : allergenes) {
			auto const new_set = std::unordered_set<std::string>(ingredient_buffer.begin(), ingredient_buffer.end());
			if (not allergense2ingrediens.contains(allergene)) {
				allergense2ingrediens.insert( { allergene, new_set });
			} else {
				auto const copy = allergense2ingrediens[allergene];
				for (auto const &ing : copy) {
					if (not new_set.contains(ing)) {
						allergense2ingrediens[allergene].erase(ing);
					}
				}
			}

		}

	}

	return std::make_tuple(allergense2ingrediens, all_ingredients);
}

auto find_possible_allergenes(auto const &allergense2ingrediens) {
	std::unordered_set<std::string> possible_allergenes { };
	for (auto &a2i : allergense2ingrediens) {
		for (auto &ing : a2i.second) {
			possible_allergenes.insert(ing);
		}
	}
	return possible_allergenes;
}

auto count_allergene_free(auto const &all_ingredients, auto const &possible_allergenes) {
	auto cnt { 0U };
	for (auto const &ing : all_ingredients) {
		if (not possible_allergenes.contains(ing)) {
			cnt++;
		}
	}
	return cnt;
}

auto& find_allergne_mapping(auto &allergense2ingrediens) {
	std::deque<std::string> deq { };
	for (auto &i : allergense2ingrediens) {
		if (i.second.size() == 1U) {
			deq.push_back(*i.second.begin());
			break;
		}
	}
	while (not deq.empty()) {
		auto cur_ing = deq.front();
		deq.pop_front();
		for (auto &i : allergense2ingrediens) {
			if (i.second.size() > 1U and i.second.contains(cur_ing)) {
				i.second.erase(cur_ing);
				if (i.second.size() == 1U) {
					deq.push_back(*i.second.begin());
				}
			}
		}
	}
	return allergense2ingrediens;
}

auto get_allergenes_ingredients_ordered(auto const &allergense2ingrediens) {
	std::vector<std::string> allergenes { allergense2ingrediens.size() };
	ranges::transform(allergense2ingrediens, allergenes.begin(), [](auto &element) {
		return element.first;
	});
	std::sort(allergenes.begin(), allergenes.end());
	std::string part2 { };
	for (auto const &a : allergenes) {
		for (auto &ing : allergense2ingrediens.at(a)) {
			part2 += ing;
		}
		part2 += ",";
	}
	//remove trailing ,
	part2.pop_back();
	return part2;
}

int main() {
	constexpr auto file_name = "build/input/input_21.txt";
	auto const lines = AOC::parse_lines(file_name);
	auto [allergense2ingrediens, all_ingredients] = parse_input(lines);
	auto possible_allergenes = find_possible_allergenes(allergense2ingrediens);
	auto part1 = count_allergene_free(all_ingredients, possible_allergenes);
	allergense2ingrediens = find_allergne_mapping(allergense2ingrediens);
	auto part2 = get_allergenes_ingredients_ordered(allergense2ingrediens);
	fmt::print("Part 1: {}\n", part1);
	fmt::print("Part 2: {}\n", part2);
	return EXIT_SUCCESS;
}

