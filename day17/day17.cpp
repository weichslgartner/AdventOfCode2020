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
constexpr auto ACTIVE { '#' };
constexpr auto INACTIVE { '.' };

struct Borders {
	int z_min;
	int z_max;
	int y_min;
	int y_max;
	int x_min;
	int x_max;
};

std::vector<Point3D> get_neighbours(Point3D const &p, int distance) {
	std::vector<Point3D> neighbors;
	Point3D next { };
	for (int z { -distance }; z <= distance; z++) {
		for (int y { -distance }; y <= distance; y++) {
			for (int x { -distance }; x <= distance; x++) {
				next.z = p.z + z;
				next.y = p.y + y;
				next.x = p.x + x;
				if (p.manhattan_distance(next) != 0) {
					neighbors.push_back(next);
				}

			}

		}

	}
	return neighbors;
}

auto get_number_active(std::vector<Point3D> &neighbors, std::unordered_map<Point3D, char> &space_map) {
	auto active { 0U };
	for (auto const neighbor : neighbors) {
		if (space_map.contains(neighbor) and space_map[neighbor] == ACTIVE) {
			active++;
		}
	}
	return active;
}

void adapt_borders(int z, int x, int y, Borders &border) {
	if (z < border.z_min) {
		border.z_min = z;
	}
	if (z > border.z_max) {
		border.z_max = z;
	}
	if (x < border.x_min) {
		border.x_min = x;
	}
	if (x > border.x_max) {
		border.x_max = x;
	}
	if (y < border.y_min) {
		border.y_min = y;
	}
	if (y > border.y_max) {
		border.y_max = y;
	}
}

void print_grid(Borders &border, std::unordered_map<Point3D, char> &space_map) {
	for (int z { border.z_min - 1 }; z <= border.z_max + 1; ++z) {
		fmt::print("z={}\n",z);
		for (int y { border.y_min - 1 }; y <= border.y_max + 1; ++y) {
			for (int x { border.x_min - 1 }; x <= border.x_max + 1; ++x) {
				if (space_map.contains(Point3D { x, y, z })) {
					fmt::print("#");
				} else {
					fmt::print(".");
				}

			}
			fmt::print("\n");
		}
		fmt::print("\n");
	}


}

auto apply_rules(Borders &border, Borders &next_borders, std::unordered_map<Point3D, char> &space_map, std::unordered_map<Point3D, char> &next) {
	auto active_cnt { 0U };

	for (int z { border.z_min - 1 }; z <= border.z_max + 1; ++z) {
		for (int y { border.y_min - 1 }; y <= border.y_max + 1; ++y) {
			for (int x { border.x_min - 1 }; x <= border.x_max + 1; ++x) {
				Point3D const p { x, y, z };
				auto neighbors = get_neighbours(p, 1);
				auto active = get_number_active(neighbors, space_map);
				//fmt::print("{} {} {}", z, y, x);
				//fmt::print("neighbor_size: {} active {}\n", neighbors.size(), active);
				if ((space_map.contains(p) and active == 2) or active == 3) {
					next[p] = ACTIVE;
					active_cnt++;
					adapt_borders(z, x, y, next_borders);
				}

			}
		}

	}
	return active_cnt;
}

auto run_til_stable(Borders &border, std::unordered_map<Point3D, char> &space_map) {
	auto occ_ov { 0 };
	auto next = space_map;
	Borders next_borders = border;
	auto i { 0 };
	while (i < 6) {
		//grid = next;
		fmt::print("iteration {}\n",i);
		std::ranges::swap(space_map, next);
		border = next_borders;
		print_grid(border, space_map);
		next.clear();
		occ_ov = apply_rules(border, next_borders, space_map, next);
		i++;
	}
	return occ_ov;
}

int main() {
	constexpr auto file_name = "build/input/input_17.txt";
	auto grid = AOC::parse_grid<char>(file_name);
	//print_grid(grid);
	//print_grid(grid);
	Borders border { };
	std::unordered_map<Point3D, char> space_map { };
	auto y { 0 };
	auto x { 0 };
	auto z { 0 };
	for (auto const &line : grid) {
		x = 0;
		for (auto const c : line) {
			if (c == ACTIVE) {
				Point3D p { x, y, z };
				space_map.insert( { p, ACTIVE });
			}
			fmt::print("{}", c);
			adapt_borders(z, x, y, border);
			x++;
		}
		fmt::print("\n");
		y++;
	}
	auto part1 = run_til_stable(border, space_map);
	fmt::print("Part 1: {}\n", part1);
	//occ_ov = run_til_stable(grid, false);
	fmt::print("Part 2: {}\n", 0);
	return EXIT_SUCCESS;
}
