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
namespace ranges = std::ranges;
struct food {
	std::vector<std::string> ingredients;
	std::unordered_set<std::string> allergenes;
};

bool check_mapping(std::vector<food>const &foods, std::unordered_map<std::string, std::string> const &mapping) {
	std::unordered_map<std::string, int> allergene_cnt { };
	for (auto const &food : foods) {
		allergene_cnt.clear();
		//auto keys = std::ranges::for_each(food.allergenes,[](auto &element){return element;});
		for (auto &ing : food.ingredients) {
			if (mapping.contains(ing)) {
				allergene_cnt[mapping.at(ing)]++;
				fmt::print("{}: {} {}\n", ing, mapping.at(ing), allergene_cnt[mapping.at(ing)]);
			}
		}
		if (allergene_cnt.size() != food.allergenes.size()) {
			//return false;
		}
		for (auto const &al : food.allergenes) {
			if (not allergene_cnt.contains(al)) {
				fmt::print("not found {}\n", al);
				return false;
			}
			if (allergene_cnt[al] > 1) {
				return false;
			}
		}

	}
	return true;
}

auto parse_input(std::vector<std::string> const &lines) {
	std::stringstream ss;
	std::string token;

	std::vector<food> foods;
	std::unordered_map<std::string, std::unordered_set<std::string> > ingrediens2allergense;
	std::unordered_map<std::string, std::unordered_set<std::string> > allergense2ingrediens;
	std::vector<std::string> ingredient_buffer { };
	std::vector<std::string> all_ingredients { };
	std::unordered_set<std::string> allergenes { };
	for (auto const &line : lines) {
		ingredient_buffer.clear();
		bool is_ingredient { true };
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
				fmt::print("{}\n", token);
			} else {
				allergenes.insert(token.substr(0, token.size() - 1));
				fmt::print("{}\n", token.substr(0, token.size() - 1));
			}
		}
		foods.push_back( { ingredient_buffer, allergenes });
		for (auto const &allergene : allergenes) {
			auto new_set =  std::unordered_set<std::string>(ingredient_buffer.begin(), ingredient_buffer.end()) ;
			if (not allergense2ingrediens.contains(allergene)) {
				allergense2ingrediens.insert( { allergene,new_set});
			} else {
				auto copy =allergense2ingrediens[allergene];
				for (auto  const &ing : copy) {
					if(not new_set.contains(ing)){
						allergense2ingrediens[allergene].erase(ing);
					}
				}
			}

		}
		/*
		 for (auto const &ingredient : ingredient_buffer) {
		 if (not ingrediens2allergense.contains(ingredient)) {
		 ingrediens2allergense.insert({ingredient,allergenes});
		 }else{
		 auto aller = ingrediens2allergense[ingredient];
		 for(auto &a: aller){
		 if(not allergenes.contains(a)){
		 ingrediens2allergense[ingredient].erase(a);
		 }

		 }
		 }
		 }*/

	}

	return std::make_tuple(allergense2ingrediens, all_ingredients, foods);
}

int main() {

	constexpr auto file_name = "build/input/input_21.txt";
	auto lines = AOC::parse_lines(file_name);
	lines.push_back("");
	auto [ingrediens2allergense, all_ingredients, foods] = parse_input(lines);
	std::unordered_set<std::string> possible_allergenes{};
	for (auto &i2a : ingrediens2allergense) {
		fmt::print("{} {} ", i2a.first, i2a.second.size());
		for (auto &i : i2a.second) {
			fmt::print("{}  ", i);
			possible_allergenes.insert(i);
		}
		fmt::print("\n");
	}
	auto cnt{0U};
	for(auto const &ing : all_ingredients){
		if(not possible_allergenes.contains(ing)){
			cnt++;
		}
	}

	//auto res = check_mapping(foods, { { "fvjkl", "soy" }, { "kfcds", "dairy" }, { "sqjhc", "fish" } });

	//fmt::print("res: {}\n", res);
	fmt::print("Part 1: {}\n", cnt);
	fmt::print("Part 2: {}\n", 0);
	return EXIT_SUCCESS;
}

