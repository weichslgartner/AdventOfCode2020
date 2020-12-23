#include "common.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fmt/core.h>
#include <vector>
#include <string>
#include <cstdint>
#include <list>
namespace ranges = std::ranges;


void print_cups(unsigned int current_cup, auto const &cups) {
	auto j { 0U };
	for (auto c : cups) {
		if (c == current_cup) {
			fmt::print("({}) ", c);
		} else {
			fmt::print("{} ", c);
		}
		j++;
	}
	fmt::print("\n");

}



std::list<unsigned>::iterator circularNext(std::list<unsigned> &list, std::list<unsigned>::iterator const &it) {
	return std::next(it) == list.end() ? list.begin() : std::next(it);
}




void crab_game_list(auto &cups, int const iterations) {
	auto current_cup_it = cups.begin();
	std::vector<std::list<unsigned>::iterator> dest_lookup{cups.size()+1};
	for (auto iter = cups.begin(); iter != cups.end(); std::advance(iter, 1)) {
		dest_lookup[*iter]=iter;
	}
	auto current_cup = cups.front();
	auto const length = cups.size();
	auto dest_it = current_cup_it;
	for (auto i { 1 }; i <= iterations; ++i) {
		auto destination { current_cup - 1 };
		auto pick_up_it = circularNext(cups, current_cup_it);
		auto pick_up_it_2 = circularNext(cups, pick_up_it);
		auto pick_up_it_3 = circularNext(cups, pick_up_it_2);
		while ((destination == *pick_up_it) or (destination == *pick_up_it_2) or (destination == *pick_up_it_3) or (destination == 0)) {
			if (destination == 0) {
				destination = length;
			} else {
				destination = (destination - 1) % length;
			}
		}
		dest_it = dest_lookup[destination];
		dest_it = circularNext(cups, dest_it);
		for (int j = 0; j < 3; ++j) {
			cups.splice(dest_it, cups, circularNext(cups, current_cup_it));
		}
		current_cup_it = circularNext(cups, current_cup_it);
		current_cup = *current_cup_it;
	}
}
auto calc_part2(auto &cups) {
	auto one_pos = std::find(cups.begin(), cups.end(), 1U);
	one_pos = circularNext(cups, one_pos);
	auto const n1 = static_cast<uint64_t>(*one_pos);
	one_pos = circularNext(cups, one_pos);
	auto const n2 = static_cast<uint64_t>(*one_pos);
	return n1 * n2;
}

std::string calc_part1(auto &cups) {
	std::string result { };
	auto one_pos = std::find(cups.begin(), cups.end(), 1U);
	auto it = circularNext(cups, one_pos);
	while (it != one_pos) {
		auto const c = '0' + *it;
		result += c;
		it = circularNext(cups, it);
	}
	return result;
}

std::list<unsigned> parse_input(auto const &input) {
	std::list<unsigned> cups{};
	for (auto i : input) {
		cups.push_back(i - '0');
	}
	return cups;
}

int main() {
	auto const input = std::string { "624397158" };
	//auto input = std::string { "389125467" };
	auto cups = parse_input(input);
	crab_game_list(cups, 100);
	auto result = calc_part1(cups);
	fmt::print("Part 1: {}\n", result);
	cups = parse_input(input);
	auto max_el = *std::max_element(cups.begin(), cups.end());
	for (auto i { max_el + 1 }; i <= 1000000; ++i) {
		cups.push_back(i);
	}
	crab_game_list(cups, 10000000);
	fmt::print("Part 2: {}\n", calc_part2(cups));
	return EXIT_SUCCESS;
}

