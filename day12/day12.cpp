#include "common.h"
#include <algorithm>
#include <array>
#include <assert.h>
#include <deque>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

enum class Cardinals {
	EAST, SOUTH, WEST, NORTH
};

struct NavInst {
	char dir;
	unsigned long value;
};

std::vector<NavInst> parse_instructions(char const *file_name) {
	std::ifstream infile { std::string(file_name) };
	if (!infile.is_open()) {
		fmt::print(stderr, "Cannot open {}\n", file_name);
		return {};
	}
	std::string line;
	std::stringstream ss { };
	ss << infile.rdbuf();
	std::vector<NavInst> ints;
	while (std::getline(ss, line)) {
		auto dir = line[0];
		auto val = std::stoul(line.substr(1, line.size() - 1));
		fmt::print("{} {}\n", dir, val);
		ints.emplace_back(NavInst { dir, val });

	}
	return ints;
}

Point card2coord(Cardinals card) {
	switch (card) {
	case Cardinals::EAST:

		return Point { 1, 0 };
	case Cardinals::WEST:

		return Point { -1, 0 };
	case Cardinals::SOUTH:
		return Point { 0, -1 };
	case Cardinals::NORTH:
		return Point { 0, 1 };

	}
	//never reached
	return{};
}

Point& move(Point &point, char action, Cardinals card, int distance) {
	auto next = card2coord(card);
	switch (action) {
	case 'F':
		next = card2coord(card);
		break;
	case 'N':
		next = card2coord(Cardinals::NORTH);
		break;
	case 'E':
		next = card2coord(Cardinals::EAST);
		break;
	case 'S':
		next = card2coord(Cardinals::SOUTH);
		break;
	case 'W':
		next = card2coord(Cardinals::WEST);
		break;

	default:
		break;
	}
	point.x += distance * next.x;
	point.y += distance * next.y;
	return point;
}

Cardinals turn(Cardinals cur_dir, char turn_dir, int degree) {
	if (degree % 90 != 0) {
		fmt::print(stderr, "degree not multiple of 90 (given {})\n", degree);
	}
	auto times = degree / 90;
	auto cur_dir_int = static_cast<int>(cur_dir);
	switch (turn_dir) {
	case 'R':
		cur_dir_int = (cur_dir_int + times) % 4;
		break;
	case 'L':
		cur_dir_int = (cur_dir_int - times) % 4;
		break;
	default:
		fmt::print(stderr, "Use R or L not {}\n", turn_dir);
		break;
	}
	if (cur_dir_int < 0) {
		cur_dir_int = 4 + cur_dir_int;
	}
	return static_cast<Cardinals>(cur_dir_int);
}

void tests() {
	assert(turn(Cardinals::EAST, 'R', 90) == Cardinals::SOUTH);
	assert(turn(Cardinals::EAST, 'L', 90) == Cardinals::NORTH);
	assert(turn(Cardinals::EAST, 'L', 180) == Cardinals::WEST);
	assert(turn(Cardinals::EAST, 'R', 180) == Cardinals::WEST);
	assert(turn(Cardinals::SOUTH, 'R', 270) == Cardinals::EAST);
	assert(turn(Cardinals::SOUTH, 'L', 270) == Cardinals::WEST);
	Point p{0,0};
	auto card = Cardinals::EAST;
	auto res = move(p, 'F', card, 10);
	assert(res.x==10 and res.y==0);
	res = move(res, 'N', card, 3);
	assert(res.x==10 and res.y==3);
}

int main() {
	constexpr auto file_name = "build/input/input_12.txt";
	tests();
	auto instructions = parse_instructions(file_name);
	Point p { 0, 0 };
	Cardinals cur_dir { Cardinals::EAST };
	for (auto inst : instructions) {
		switch (inst.dir) {
		case 'R':
		case 'L':
			cur_dir = turn(cur_dir, inst.dir, inst.value);
			break;
		default:
			p =  move(p, inst.dir, cur_dir, inst.value);
			break;
		}
	}
	//print_grid(grid);
	fmt::print("Part 1: {}\n", p.manhattan_distance());
	fmt::print("Part 2: {}\n", "");
	return EXIT_SUCCESS;
}
