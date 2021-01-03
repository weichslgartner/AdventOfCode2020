#include "common.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fmt/core.h>
#include <vector>
#include <string>
#include <cstdint>
#include <list>
#include <iterator>
#include <string_view>
using namespace std::literals;
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

auto circularNext(auto &list, auto const &it) {
	return std::next(it) == list.end() ? list.begin() : std::next(it);
}

void crab_game_list(auto &cups, int const iterations) {
	auto current_cup_it = cups.begin();
	std::vector<std::list<unsigned>::iterator> dest_lookup { cups.size() + 1 };
	for (auto iter = cups.begin(); iter != cups.end(); std::advance(iter, 1)) {
		dest_lookup[*iter] = iter;
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

auto crab_game_vector(auto &cups, int const iterations) {
	std::vector<unsigned> dest_lookup { };
	dest_lookup.resize(cups.size() + 1);
	for (auto i { 0U }; i < cups.size() - 1; ++i) {
		dest_lookup[cups[i]] = cups[i + 1];
	}
	dest_lookup[cups[cups.size() - 1]] = cups[0];
	auto current_cup = cups.front();
	auto const length = cups.size();
	for (auto i { 1 }; i <= iterations; ++i) {
		auto destination { current_cup - 1 };
		auto pick_up_it = dest_lookup[current_cup];
		auto pick_up_it_2 = dest_lookup[pick_up_it];
		auto pick_up_it_3 = dest_lookup[pick_up_it_2];
		while ((destination == pick_up_it) or (destination == pick_up_it_2) or (destination == pick_up_it_3) or (destination == 0)) {
			if (destination == 0) {
				destination = length;
			} else {
				destination = (destination - 1) % length;
			}
		}
		dest_lookup[current_cup] = dest_lookup[pick_up_it_3];
		auto temp = dest_lookup[destination];
		dest_lookup[destination] = pick_up_it;
		dest_lookup[pick_up_it_3] = temp;
		current_cup = dest_lookup[current_cup];
	}

	return dest_lookup;
}

auto calc_part2(auto &cups) {
	auto one_pos = std::find(cups.begin(), cups.end(), 1U);
	one_pos = circularNext(cups, one_pos);
	auto const n1 = static_cast<uint64_t>(*one_pos);
	one_pos = circularNext(cups, one_pos);
	auto const n2 = static_cast<uint64_t>(*one_pos);
	return n1 * n2;
}

auto calc_part2_vector(auto &cups) {
	auto const n1 = static_cast<uint64_t>(cups[1U]);
	auto const n2 = static_cast<uint64_t>(cups[cups[1U]]);
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

std::string calc_part1_vec(auto &cups) {
	std::string result { };
	auto element = cups[1];
	while (element != 1) {
		result += '0' + element;
		element = cups[element];
	}
	return result;
}

template<class T>
T parse_input(std::string_view input) {
	T cups { };
	ranges::transform(input,std::back_inserter(cups),[](auto const i){return i - '0';});
	return cups;
}

int main() {
	auto const input { "624397158"sv };
	auto const use_linked_list { true };
	auto const iterations_part1{100};
	auto const iterations_part2{10000000};
	auto const n_cups_part2{1000000};
	//auto const input  { "389125467"sv };
	if (use_linked_list) {
		auto cups =  parse_input<std::list<unsigned>>(input);
		crab_game_list(cups, iterations_part1);
		auto result = calc_part1(cups);
		fmt::print("Part 1: {}\n", result);
		cups = parse_input<std::list<unsigned>>(input);
		auto max_el = *std::max_element(cups.begin(), cups.end());
		for (auto i { max_el + 1 }; i <= n_cups_part2; ++i) {
			cups.push_back(i);
		}
		crab_game_list(cups, iterations_part2);
		fmt::print("Part 2: {}\n", calc_part2(cups));
	} else {
		auto cups = parse_input<std::vector<unsigned>>(input);
		auto p1 = crab_game_vector(cups, iterations_part1);
		auto result = calc_part1_vec(p1);
		fmt::print("Part 1: {}\n", result);
		cups = parse_input<std::vector<unsigned>>(input);
		auto max_el = *std::max_element(cups.begin(), cups.end());
		cups.reserve(n_cups_part2);
		for (auto i { max_el + 1 }; i <= n_cups_part2; ++i) {
			cups.push_back(i);
		}
		auto p2 = crab_game_vector(cups, iterations_part2);
		fmt::print("Part 2: {}\n", calc_part2_vector(p2));
	}
	return EXIT_SUCCESS;
}

