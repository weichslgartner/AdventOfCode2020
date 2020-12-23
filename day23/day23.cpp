#include "common.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fmt/core.h>
#include <vector>
#include <string>
#include <cstdint>
#include <list>

#include <utility>
namespace ranges = std::ranges;

void print_cups(unsigned int current_cup_pos, auto const &cups) {
	auto j { 0U };
	for (auto c : cups) {
		if (j == current_cup_pos) {
			fmt::print("({}) ", c);
		} else {
			fmt::print("{} ", c);
		}
		j++;
	}
	fmt::print("\n");

}

void print_cups_v2(unsigned int current_cup, auto const &cups) {
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

void pickup(std::vector<unsigned> const &cups, unsigned int current_cup_pos, unsigned length, std::vector<unsigned> &pick_up) {
	pick_up.clear();
	//fmt::print("Pick up: ");
	for (auto j { 1 }; j < 4; j++) {
		auto tmp = cups[(current_cup_pos + j) % length];
		pick_up.push_back(tmp);
		//fmt::print("{} ", tmp);
	}
	//fmt::print("\n");
}

void crab_game(auto &cups, int iterations) {
	auto current_cup_pos { 0U };
	std::vector<unsigned> pick_up { };
	auto current_cup = cups[current_cup_pos];
	auto length = cups.size();
	for (auto i { 1 }; i <= iterations; ++i) {

		//print_cups(current_cup_pos,cups);
		pickup(cups, current_cup_pos, length, pick_up);
		cups.erase(std::remove_if(cups.begin(), cups.end(), [&pick_up](auto const x) {
			return std::find(pick_up.begin(), pick_up.end(), x) != pick_up.end();
		}),cups.end());
		//fmt::print("cups size: {}\n", cups.size());
		//print_cups(current_cup_pos,cups);
		auto destination { current_cup - 1 };
		while (std::find(pick_up.begin(), pick_up.end(), destination) != pick_up.end() or destination == 0) {
			if (destination == 0) {
				destination = length;
			} else {
				destination = (destination - 1) % length;
			}
			auto insert_point = std::find(cups.begin(), cups.end(), destination);
			cups.insert(insert_point + 1, pick_up.begin(), pick_up.end());
			auto next_cup = std::find(cups.begin(), cups.end(), current_cup);
			if (next_cup < cups.end() - 1) {
				current_cup = *(next_cup + 1);

			} else {
				current_cup = cups.front();

			}
			current_cup_pos = (current_cup_pos + 1) % length;
			while (cups[current_cup_pos] != current_cup) {
				std::rotate(cups.begin(), cups.begin() + 1, cups.end());
			}

		}
	}
}

std::list<unsigned>::iterator circularNext(std::list<unsigned> &l, std::list<unsigned>::iterator &it) {
	return std::next(it) == l.end() ? l.begin() : std::next(it);
}

std::list<unsigned>::iterator circularLast(std::list<unsigned> &l, std::list<unsigned>::iterator &it) {
	return it == l.begin() ? std::prev(l.end()) : std::prev(it);
}

void crab_game_list(std::list<unsigned> &cups, int const iterations) {
	std::list<unsigned>::iterator current_cup_it = cups.begin();
	std::unordered_map<unsigned, std::list<unsigned>::iterator> dest_lookup;
	for (auto iter = cups.begin(); iter != cups.end(); std::advance(iter, 1)) {
		dest_lookup.insert( { *iter, iter });
	}
	auto current_cup = cups.front();
	auto const length = cups.size();
	auto dest_it = current_cup_it;
	for (auto i { 1 }; i <= iterations; ++i) {
		auto destination { current_cup - 1 };
		auto pick_up_it = circularNext(cups, current_cup_it);
		auto pick_up_it_2 = circularNext(cups, pick_up_it);
		auto pick_up_it_3 = circularNext(cups, pick_up_it_2);

		while ((destination == *pick_up_it) or (destination == *pick_up_it_2) or (destination == *pick_up_it_3) or destination == 0) {
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
	//print_cups(current_cup_pos,cups);
	std::string result { };
	auto one_pos = std::find(cups.begin(), cups.end(), 1);
	one_pos = circularNext(cups, one_pos);
	auto const n1 = static_cast<uint64_t>(*one_pos);
	one_pos = circularNext(cups, one_pos);
	auto const n2 = static_cast<uint64_t>(*one_pos);
	return n1 * n2;
}

std::string calc_part1(auto &cups) {
	//print_cups(current_cup_pos,cups);
	std::string result { };
	auto one_pos = std::find(cups.begin(), cups.end(), 1);
	while (true) {
		one_pos++;
		if (one_pos == cups.end()) {
			one_pos = cups.begin();
		}
		auto const c = '0' + *one_pos;
		if (c == '1') {
			break;
		}
		result += c;
	}
	return result;
}

int main() {

	auto const input = std::string { "624397158" };
	//auto input = std::string { "389125467" };
	std::list<unsigned> cups;
	for (auto i : input) {
		cups.push_back(i - '0');
	}
	auto cups_cpy = cups;
	crab_game_list(cups, 100);
	auto result = calc_part1(cups);
	fmt::print("Part 1: {}\n", result);
	cups = cups_cpy;
	auto max_el = *std::max_element(cups.begin(), cups.end());
	for (auto i { max_el + 1 }; i <= 1000000; ++i) {
		cups.push_back(i);
	}
	crab_game_list(cups, 10000000);
	fmt::print("Part 2: {}\n", calc_part2(cups));
	return EXIT_SUCCESS;
}

