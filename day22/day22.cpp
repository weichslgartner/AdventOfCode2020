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
#include <cctype>
#include <utility>
namespace ranges = std::ranges;

auto parse_input(auto const &lines) {
	std::deque<unsigned> player1;
	std::deque<unsigned> player2;
	bool parse_player1 { true };
	for (auto &line : lines) {
		if (line.empty()) {
			parse_player1 = false;
		}
		if (std::isdigit(line[0])) {
			auto number = std::stoul(line);
			if (parse_player1) {
				player1.push_back(number);
			} else {
				player2.push_back(number);
			}
		}

	}

	return std::make_pair(player1, player2);
}

auto calc_result(std::deque<unsigned> &player) {
	auto result { 0U };
	auto i { 1U };
	while (not player.empty()) {
		auto card = player.back();
		//fmt::print("{}\n",card);
		result += i * card;
		player.pop_back();
		i++;
	}
	return result;
}

void play_game(std::deque<unsigned> &player1, std::deque<unsigned> &player2) {
	while (not player1.empty() and not player2.empty()) {
		auto const card1 = player1.front();
		player1.pop_front();
		auto const card2 = player2.front();
		player2.pop_front();
		if (card1 > card2) {
			player1.push_back(card1);
			player1.push_back(card2);
		} else {
			player2.push_back(card2);
			player2.push_back(card1);
		}

	}
}

int main() {
	constexpr auto file_name = "build/input/input_22.txt";
	auto const lines = AOC::parse_lines(file_name);
	auto [player1, player2] = parse_input(lines);
	play_game(player1, player2);
	auto result { 0U };
	if (not player2.empty()) {
		result = calc_result(player2);
	} else {
		result = calc_result(player1);
	}
	fmt::print("Part 1: {}\n", result);
	fmt::print("Part 2: {}\n", player2.size());
	return EXIT_SUCCESS;
}

