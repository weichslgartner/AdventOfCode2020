#include "common.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <fmt/core.h>
#include <vector>
#include <string>
#include <cstdint>
#include <list>
#include <iterator>
#include <unordered_set>
namespace ranges = std::ranges;

enum class Dir {
	EAST, SOUTHEAST, SOUTHWEST, WEST, NORTHWEST, NORTHEAST

};

Point& get_neighbor(Point &p, Dir dir) {
	switch (dir) {
	case Dir::EAST:
		p.x++;
		break;
	case Dir::SOUTHEAST:
		p.y++;
		break;
	case Dir::SOUTHWEST:
		p.x--;
		p.y++;
		break;
	case Dir::WEST:
		p.x--;
		break;
	case Dir::NORTHWEST:
		p.y--;
		break;
	case Dir::NORTHEAST:
		p.x++;
		p.y--;
		break;
	default:
		fmt::print(stderr, "Parsing error");
		break;
	}
	return p;
}

auto count_black(std::vector<Point> points, auto const &black_tiles) {
	return ranges::count_if(points,[&black_tiles](auto const &p){return black_tiles.contains(p);});

}

auto get_whites(std::vector<Point> points, auto const &black_tiles) {
	std::vector<Point> whites { };
	for (auto point : points) {
		if (not black_tiles.contains(point)) {
			whites.push_back(point);
		}
	}
	return whites;
}

std::vector<Point> get_all_neighbors(Point const &p) {
	std::vector<Point> neighbors { };
	std::array const directions { Dir::EAST, Dir::SOUTHEAST, Dir::SOUTHWEST, Dir::WEST, Dir::NORTHWEST, Dir::NORTHEAST };
	for (auto dir : directions) {
		Point n { p.x, p.y };
		neighbors.push_back(get_neighbor(n, dir));
	}
	return neighbors;
}

auto parse_input(auto &lines) {
	std::vector<std::vector<Dir>> paths { };
	for (auto const &line : lines) {
		std::vector<Dir> path;
		for (auto i { 0U }; i < line.size(); ++i) {
			if (line[i] == 'e') {
				path.push_back(Dir::EAST);
			} else if (line[i] == 'w') {
				path.push_back(Dir::WEST);
			} else if (line[i] == 's') {
				if (line[i + 1] == 'e') {
					path.push_back(Dir::SOUTHEAST);
				} else if (line[i + 1] == 'w') {
					path.push_back(Dir::SOUTHWEST);
				} else {
					fmt::print(stderr, "Parsing error");
				}
				i++;
			} else if (line[i] == 'n') {
				if (line[i + 1] == 'e') {
					path.push_back(Dir::NORTHEAST);
				} else if (line[i + 1] == 'w') {
					path.push_back(Dir::NORTHWEST);
				} else {
					fmt::print(stderr, "Parsing error");
				}
				i++;
			} else {
				fmt::print(stderr, "Parsing error");

			}
		}
		paths.push_back(path);
	}
	return paths;
}

std::unordered_set<Point> flip_tiles_part1(auto const &paths) {
	std::unordered_set<Point> black_tiles;
	for (auto const &path : paths) {
		Point p { 0, 0 };
		for (auto dir : path) {
			p = get_neighbor(p, dir);
		}
		if (black_tiles.contains(p)) {
			black_tiles.erase(p);
		} else {
			black_tiles.insert(p);
		}
	}
	return black_tiles;
}

auto& convert_to_white(auto &neighbors, std::unordered_set<Point> &black_tiles, std::unordered_set<Point> &flip_to_white, Point const &tile) {
	auto blk_cnt = count_black(neighbors, black_tiles);
	if (blk_cnt == 0 or blk_cnt > 2) {
		flip_to_white.insert(tile);
	}
	return flip_to_white;
}

auto& convert_to_black(std::unordered_set<Point> &flip_to_black, auto &neighbors, std::unordered_set<Point> &black_tiles) {
	auto whites = get_whites(neighbors, black_tiles);
	for (auto &white_tile : whites) {
		auto w_neigh = get_all_neighbors(white_tile);
		auto blk_cnt = count_black(w_neigh, black_tiles);
		if (blk_cnt == 2) {
			flip_to_black.insert(white_tile);
		}
	}
	return flip_to_black;

}

void part2(auto &black_tiles, auto iterations) {
	std::unordered_set<Point> flip_to_white { };
	std::unordered_set<Point> flip_to_black { };
	for (auto i { 1 }; i <= iterations; ++i) {
		flip_to_white.clear();
		flip_to_black.clear();
		for (auto &tile : black_tiles) {
			auto neighbors = get_all_neighbors(tile);
			flip_to_white = convert_to_white(neighbors, black_tiles, flip_to_white, tile);
			flip_to_black = convert_to_black(flip_to_black, neighbors, black_tiles);
		}
		black_tiles.insert(flip_to_black.begin(), flip_to_black.end());
		std::erase_if(black_tiles, [&flip_to_white](auto const &x) {
			return flip_to_white.contains(x);
		});
	}
}

int main() {
	constexpr auto file_name = "build/input/input_24.txt";
	auto const lines = AOC::parse_lines(file_name);
	auto paths = parse_input(lines);
	auto black_tiles = flip_tiles_part1(paths);
	fmt::print("Part 1: {}\n", black_tiles.size());
	part2(black_tiles,100);
	fmt::print("Part 2: {}\n", black_tiles.size());
	return EXIT_SUCCESS;
}

