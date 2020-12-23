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
#include <cstddef>
#include <functional>
#include <utility>
namespace ranges = std::ranges;

class DeqHash {
public:
	std::size_t operator()(const std::deque<unsigned> &v) const {
		std::size_t x = 0;
		for (auto const i : v)
			x ^= std::hash<unsigned>()(i);
		return x;
	}
};

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
		auto const card = player.back();
		result += i * card;
		player.pop_back();
		i++;
	}
	return result;
}

bool play_game_recursive(std::deque<unsigned> player1, std::deque<unsigned> player2) {
	std::unordered_set<std::deque<unsigned>, DeqHash> memory { };
	while (not player1.empty() and not player2.empty()) {
		auto const card1 = player1.front();
		player1.pop_front();
		auto const card2 = player2.front();
		player2.pop_front();
		if (memory.contains(player1) or memory.contains(player2)) {
			return true;
		}
		bool player1_wins = card1 > card2;
		if (card1 < player1.size() and card2 < player2.size()) {
			player1_wins = play_game_recursive(std::deque<unsigned> { player1.begin(), player1.begin() + card1 }, std::deque<unsigned> {
					player2.begin(), player2.begin() + card2 });
		}
		if (player1_wins) {
			player1.push_back(card1);
			player1.push_back(card2);
		} else {
			player2.push_back(card2);
			player2.push_back(card1);
		}
		memory.insert(player1);
		memory.insert(player2);
	}
	return not player1.empty();

}

unsigned play_game(std::deque<unsigned> player1, std::deque<unsigned> player2, bool const part2) {
	while (not player1.empty() and not player2.empty()) {
		auto const card1 = player1.front();
		player1.pop_front();
		auto const card2 = player2.front();
		player2.pop_front();
		bool player1_wins = card1 > card2;
		if (card1 <= player1.size() and card2 <= player2.size() and part2) {
			player1_wins = play_game_recursive(std::deque<unsigned> { player1.begin(), player1.begin() + card1 }, std::deque<unsigned> {
					player2.begin(), player2.begin() + card2 });
		}
		if (player1_wins) {
			player1.push_back(card1);
			player1.push_back(card2);
		} else {
			player2.push_back(card2);
			player2.push_back(card1);
		}

	}
	auto result { 0U };
	if (not player2.empty()) {
		result = calc_result(player2);
	} else {
		result = calc_result(player1);
	}
	return result;
}

int main() {
	constexpr auto file_name = "build/input/input_22.txt";
	auto const lines = AOC::parse_lines(file_name);
	auto [player1, player2] = parse_input(lines);
	fmt::print("Part 1: {}\n", play_game(player1, player2, false));
	fmt::print("Part 2: {}\n", play_game(player1, player2, true));
	return EXIT_SUCCESS;
}

