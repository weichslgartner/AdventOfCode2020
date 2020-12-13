#include "common.h"
#include <algorithm>
#include <concepts>
#include <fmt/core.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <cassert>


#include <array>

namespace ranges = std::ranges;
constexpr auto EMPTY { 'L' };
constexpr auto OCCUPIED { '#' };
constexpr auto FLOOR { '.' };

constexpr auto directions = []() {
	std::array<Point, 8> array { };
	auto index { 0U };
	for (int i { -1 }; i <= 1; ++i) {
		for (int j { -1 }; j <= 1; ++j) {
			if (not (i == 0 && j == 0)) {
				array[index] = Point { i, j };
				index++;
			}

		}
	}
	return array;
}();

bool out_of_bounds(Point const point, std::vector<std::vector<char> > const &grid) {
	return grid.empty() or grid[0].empty() or point.x < 0 or point.x >= grid[0].size() or point.y < 0 or point.y >= grid.size();
}

std::vector<char> get_neighbours_part2(Point const &p, std::vector<std::vector<char>> const &grid) {
	std::vector<char> neighbors;
	Point next{};
	for (auto const &dir : directions) {
		next.x = p.x + dir.x;
		next.y = p.y + dir.y;
		while (not out_of_bounds(next, grid)) {
			char const c = grid.at(next.y).at(next.x);
			if (c == OCCUPIED or c == EMPTY) {
				neighbors.push_back(c);
				break;
			}
			next.x = next.x + dir.x;
			next.y = next.y + dir.y;
		}
	}
	return neighbors;
}

std::vector<char> get_neighbours(Point p, std::vector<std::vector<char>> const &grid) {
	std::vector<char> neighbors;
	for (int i { -1 }; i <= 1; ++i) {
		for (int j { -1 }; j <= 1; ++j) {
			if (i == 0 and j == 0) {
				continue;
			}
			Point const next { p.x + i, p.y + j };
			if (out_of_bounds(next, grid)) {
				continue;
			}
			char const c = grid.at(next.y).at(next.x);
			neighbors.push_back(c);
		}
	}
	return neighbors;
}

void print_grid(std::vector<std::vector<char>> const &grid) {
	for (auto const &line : grid) {
		for (auto const c : line) {
			fmt::print("{}", c);
		}
		fmt::print("\n");
	}

}

auto apply_rules(std::vector<std::vector<char>> &grid, std::vector<std::vector<char>> &next, bool part1) {
	auto occ_overall { 0U };
	std::vector<char> neighbours { };
	auto condition { 4 };
	if (not part1) {
		condition = 5;
	}
	for (int y { 0 }; y < grid.size(); ++y) {
		for (int x { 0 }; x < grid.at(y).size(); ++x) {
			Point const p { x, y };
			auto const cur_state = grid.at(y).at(x);
			auto nex_state = cur_state;

			if (part1) {
				neighbours = get_neighbours(p, grid);

			} else {
				neighbours = get_neighbours_part2(p, grid);
			}
			auto num_occ = ranges::count_if(neighbours, [](char const x) {
				return x == OCCUPIED;
			});
			switch (cur_state) {
			case EMPTY:
				if (num_occ == 0) {
					nex_state = OCCUPIED;
				}
				break;
			case OCCUPIED:
				if (num_occ >= condition) {
					nex_state = EMPTY;
				}
				occ_overall++;
				break;
			default:
				break;
			}
			next[y][x] = nex_state;
		}
	}
	return occ_overall;
}

auto run_til_stable(std::vector<std::vector<char>> &grid, bool const part1) {
	auto occ_ov { 0 };
	auto next = grid;
	auto i { 0 };
	while (i == 0 || next != grid) {
		//grid = next;
		std::ranges::swap(grid, next);
		occ_ov = apply_rules(grid, next, part1);
		i++;
	}
	return occ_ov;
}

void tests() {
	auto const grid2 = AOC::parse_grid<char>("build/input/input_11_test2.txt");
	if (not grid2.empty()) {
		Point const start { 3, 4 };
		auto test_1 = get_neighbours_part2(start, grid2);
		auto test_cnt = ranges::count_if(test_1, [](char x) {
			return x == OCCUPIED;
		});
		assert(test_cnt == 8);
	}
}

int main() {
	constexpr auto file_name = "build/input/input_11.txt";
	auto grid = AOC::parse_grid<char>(file_name);
	//print_grid(grid);
	auto occ_ov = run_til_stable(grid, true);
	fmt::print("Part 1: {}\n", occ_ov);
	occ_ov = run_til_stable(grid, false);
	fmt::print("Part 2: {}\n", occ_ov);
	return EXIT_SUCCESS;
}
