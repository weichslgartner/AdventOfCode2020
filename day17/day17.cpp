#include "common.h"
#include <concepts>
#include <fmt/core.h>
#include <vector>
#include <ranges>
#include <unordered_set>
#include <utility>

namespace ranges = std::ranges;
constexpr auto ACTIVE { '#' };
constexpr auto INACTIVE { '.' };

struct Borders {
	int w_min;
	int w_max;
	int z_min;
	int z_max;
	int y_min;
	int y_max;
	int x_min;
	int x_max;
};

std::vector<Point4D> get_neighbours(Point4D const &p, int const distance, bool part2) {
	std::vector<Point4D> neighbors;
	Point4D next { };
	auto w_min { 0 };
	auto w_max { 0 };

	if (part2) {
		w_min = -distance;
		w_max = distance;
	}
	for (int w { w_min }; w <= w_max; w++) {
		for (int z { -distance }; z <= distance; z++) {
			for (int y { -distance }; y <= distance; y++) {
				for (int x { -distance }; x <= distance; x++) {
					next.w = p.w + w;
					next.z = p.z + z;
					next.y = p.y + y;
					next.x = p.x + x;
					if (p.manhattan_distance(next) != 0) {
						neighbors.push_back(next);
					}

				}

			}
		}
	}
	return neighbors;
}

template<class T>
auto get_number_active(std::vector<T> &neighbors, std::unordered_set<T> &space_map) {
	auto active { 0U };
	for (auto const neighbor : neighbors) {
		if (space_map.contains(neighbor)) {
			active++;
		}
	}
	return active;
}

void adapt_borders(int x, int y, int z, int w, Borders &border) {
	if (w < border.w_min) {
		border.w_min = w;
	}
	if (w > border.w_max) {
		border.w_max = w;
	}
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

void print_grid(Borders &border, std::unordered_set<Point4D> &space_map) {
	for (int w { border.w_min - 1 }; w <= border.w_max + 1; ++w) {
		for (int z { border.z_min - 1 }; z <= border.z_max + 1; ++z) {
			fmt::print("z={} w={}\n", z, w);
			for (int y { border.y_min - 1 }; y <= border.y_max + 1; ++y) {
				for (int x { border.x_min - 1 }; x <= border.x_max + 1; ++x) {
					if (space_map.contains(Point4D { x, y, z, w })) {
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
}

auto apply_rules(Borders &border, Borders &next_borders, std::unordered_set<Point4D> &space_map, std::unordered_set<Point4D> &next, bool part2) {
	auto active_cnt { 0U };
	auto w_min { 0 };
	auto w_max { 0 };
	if (part2) {
		w_min = border.w_min - 1;
		w_max = border.w_max + 1;
	}
	for (int w { border.w_min - 1 }; w <= border.w_max + 1; ++w) {
		for (int z { border.z_min - 1 }; z <= border.z_max + 1; ++z) {
			for (int y { border.y_min - 1 }; y <= border.y_max + 1; ++y) {
				for (int x { border.x_min - 1 }; x <= border.x_max + 1; ++x) {
					Point4D const p { x, y, z, w };
					auto neighbors = get_neighbours(p, 1, part2);
					auto const active = get_number_active(neighbors, space_map);
					if ((space_map.contains(p) and active == 2) or active == 3) {
						next.insert(std::move(p));
						active_cnt++;
						adapt_borders(x, y, z, w, next_borders);
					}

				}
			}
		}
	}
	return active_cnt;
}

auto run_til_stable(Borders border, std::unordered_set<Point4D> space_map, bool part2, int iterations = 6) {
	auto active_cnt { 0 };
	auto next = space_map;
	Borders next_borders = border;
	for (int i { 0 }; i < iterations; ++i) {
		std::ranges::swap(space_map, next);
		std::swap(border, next_borders);
		next.clear();
		active_cnt = apply_rules(border, next_borders, space_map, next, part2);
	}
	return active_cnt;
}

std::unordered_set<Point4D> create_space_map(std::vector<std::vector<char>> const &grid, Borders &border) {
	std::unordered_set<Point4D> space_map { };
	//space_map.reserve();
	auto y { 0 };
	auto x { 0 };
	auto z { 0 };
	auto w { 0 };
	for (auto const &line : grid) {
		x = 0;
		for (auto const c : line) {
			if (c == ACTIVE) {
				space_map.emplace(Point4D { x, y, z, w });
			}
			adapt_borders(x, y, z, w, border);
			x++;
		}
		y++;
	}
	return space_map;
}

int main() {
	constexpr auto file_name = "build/input/input_17.txt";
	auto grid = AOC::parse_grid<char>(file_name);
	Borders border { };
	auto space_map = create_space_map(grid, border);
	auto part1 = run_til_stable(border, space_map, false);
	fmt::print("Part 1: {}\n", part1);
	auto part2 = run_til_stable(border, space_map, true);
	fmt::print("Part 2: {}\n", part2);
	return EXIT_SUCCESS;
}
