#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <span>
#include <fmt/core.h>
#include <string>
#include <string_view>
#include <vector>

constexpr int find_seat(std::string_view const row, int max, char const upper, char const lower) {
	auto min { 0 };
	for (auto const c : row) {
		if (upper == c)
			min += (max - min) / 2 + 1;
		if (lower == c)
			max -= (max - min) / 2 + 1;
	}
	return max;
}

constexpr int get_seat(std::string_view const input) {
	auto row = input.substr(0, 7);
	auto column = input.substr(7, 3);
	auto row_pos = find_seat(row, 127, 'B', 'F');
	auto column_pos = find_seat(column, 7, 'R', 'L');
	return row_pos * 8 + column_pos;
}

void tests() {
	assert(get_seat("FBFBBFFRLR") == 357);
	assert(get_seat("BFFFBBFRRR") == 567);
	assert(get_seat("FFFBBBFRRR") == 119);
	assert(get_seat("BBFFBBFRLL") == 820);
}

#ifdef NDEBUG
constexpr
#endif
int find_my_seat(std::vector<int> &seats) {
	std::sort(seats.begin(), seats.end());
	auto last_seat { seats[0] - 1 };
	int myseat { 0 };
	for (auto seat : seats) {
		if ((seat - last_seat) > 1) {
			myseat = seat - 1;
			break;
		}
		last_seat = seat;
	}
	return myseat;
}

int main(int argc, char **argv) {
	tests();
	auto const *file_name = "build/input/input_05.txt";
	if (argc == 2) {
		std::span<char*> args { argv, 2U };
		file_name = args[1];
	}
	std::ifstream infile(file_name);
	if (not infile.is_open()) {
		fmt::print(stderr,"Cannot open {}\n", file_name);
		return EXIT_FAILURE;
	}
	std::string line;
	std::vector<int> seats;

	auto max { 0 };

	while (std::getline(infile, line)) {
		auto const id {get_seat(line)};
		if (id > max)
			max = id;
		seats.push_back(id);
	}
	auto myseat = find_my_seat(seats);
	fmt::print("Part 1: {}\n", max);
	fmt::print("Part 2: {}\n", myseat);
	return EXIT_SUCCESS;
}
