#include "common.h"
#include <cassert>
#include <fmt/format.h>
#include <fstream>
#include <sstream>
#include <string>
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
		ints.emplace_back(NavInst { dir, val });
	}
	return ints;
}

Point card2coord(Cardinals const card) {
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
	return {};
}

Point& move(Point &point, char const action, Cardinals const card, int const distance) {
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

Cardinals turn(Cardinals const cur_dir, char turn_dir, int degree) {
	if (degree % 90 != 0) {
		fmt::print(stderr, "degree not multiple of 90 (given {})\n", degree);
	}
	auto const times = degree / 90;
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


Point part_1(std::vector<NavInst, std::allocator<NavInst> > const &instructions, Point p) {
	Cardinals cur_dir { Cardinals::EAST };
	for (auto inst : instructions) {
		switch (inst.dir) {
		case 'R':
		case 'L':
			cur_dir = turn(cur_dir, inst.dir, inst.value);
			break;
		default:
			p = move(p, inst.dir, cur_dir, inst.value);
			break;
		}
	}
	return p;
}

Point& rotate_waypoint(Point &p, NavInst inst) {
	switch (inst.dir) {
	case 'R':
		switch (inst.value) {
		case 90:
			p = Point { p.y, -p.x };
			break;
		case 180:
			p = Point { -p.x, -p.y };
			break;
		case 270:
			p = Point { -p.y, p.x };
			break;
		default:
			fmt::print(stderr, "unkown degree");
		}
		break;
	case 'L':
		switch (inst.value) {
		case 90:
			p = Point { -p.y, p.x };
			break;
		case 180:
			p = Point { -p.x, -p.y };
			break;
		case 270:
			p = Point { p.y, -p.x };
			break;
		default:
			fmt::print(stderr, "unkown degree");
		}
		break;
	}
	return p;
}

Point part2(std::vector<NavInst, std::allocator<NavInst> > const &instructions, Point p, Point waypoint) {
	for (auto inst : instructions) {
		switch (inst.dir) {
		case 'F':
			p.x += inst.value * waypoint.x;
			p.y += inst.value * waypoint.y;
			break;
		case 'N':
			waypoint.y += inst.value;
			break;
		case 'S':
			waypoint.y -= inst.value;
			break;
		case 'W':
			waypoint.x -= inst.value;
			break;
		case 'E':
			waypoint.x += inst.value;
			break;
		case 'R':
		case 'L':
			waypoint = rotate_waypoint(waypoint, inst);
			break;
		default:
			break;
		}
	}
	return p;
}

void tests() {
	assert(turn(Cardinals::EAST, 'R', 90) == Cardinals::SOUTH);
	assert(turn(Cardinals::EAST, 'L', 90) == Cardinals::NORTH);
	assert(turn(Cardinals::EAST, 'L', 180) == Cardinals::WEST);
	assert(turn(Cardinals::EAST, 'R', 180) == Cardinals::WEST);
	assert(turn(Cardinals::SOUTH, 'R', 270) == Cardinals::EAST);
	assert(turn(Cardinals::SOUTH, 'L', 270) == Cardinals::WEST);
	Point p { 0, 0 };
	auto card = Cardinals::EAST;
	auto res = move(p, 'F', card, 10);
	assert((res.x == 10) and (res.y == 0));
	res = move(res, 'N', card, 3);
	assert((res.x == 10) and (res.y == 3));
}


int main() {
	constexpr auto file_name = "build/input/input_12.txt";
	tests();
	auto instructions = parse_instructions(file_name);
	Point p { 0, 0 };
	p = part_1(instructions, p);
	fmt::print("Part 1: {}\n",  p.manhattan_distance(Point { 0, 0 }));
	p = Point{ 0, 0 };
	Point waypoint { 10, 1 };
	p = part2(instructions, p, waypoint);
	fmt::print("Part 2: {}\n", p.manhattan_distance(Point { 0, 0 }));
	return EXIT_SUCCESS;
}
