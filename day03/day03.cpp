#include <fstream>
#include <iostream>
#include <iterator>
#include <span>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

struct Point {
  int x;
  int y;
  const bool operator==(const Point &obj) const {
    return obj.x == x && obj.y == y;
  }
};

namespace std {
template <> struct hash<Point> {
  std::size_t operator()(const Point &p) const {
    std::size_t val{0};
    val = 100000 * p.y + p.x;
    return val;
  }
};
} // namespace std

int count_trees(std::vector<Point> const &slopes,
                std::unordered_set<Point> const &trees, Point const &limits) {
  int slope_mult{1};
  int tree_count{0};
  for (auto slope : slopes) {
    int cur_x{0};
    int cur_y{0};
    tree_count = 0;
    while (cur_y <= limits.y) {
      if (trees.find(Point{cur_x, cur_y}) != trees.end()) {
        tree_count++;
      }
      cur_x += slope.x;
      if (limits.x != 0) {
        cur_x = cur_x % limits.x;
      }
      cur_y += slope.y;
    }
    slope_mult *= tree_count;
  }
  return slope_mult;
}

std::pair<std::unordered_set<Point>, Point> parse_input(char const *file_name) {
  std::ifstream infile(file_name);
  if (not infile.is_open()) {
    std::cerr << "Cannot open File: " << file_name;
    return {};
  }
  std::stringstream ss{};
  ss << infile.rdbuf();
  auto input = ss.rdbuf()->str();
  int x{0};
  int y{0};
  std::unordered_set<Point> trees;
  for (char const &a : input) {
    switch (a) {
    case '#':
      trees.insert(Point{x, y});
      x++;
      break;
    case '.':
      x++;
      break;
    case '\n':
      y++;
      x = 0;
      break;
    default:
      break;
    }
  }
  return std::pair<std::unordered_set<Point>, Point>(trees, Point{x, y});
}

int main(int argc, char **argv) {
  auto const *file_name = "build/input/input_03.txt";
  if (argc == 2) {
    std::span<char *> args{argv, 2};
    file_name = args[1];
  }
  auto [trees, limits] = parse_input(file_name);
  std::vector<Point> slopes;
  slopes.emplace_back(Point{3, 1});
  std::cout << "Part 1: " << count_trees(slopes, trees, limits) << "\n";
  slopes.clear();
  slopes.emplace_back(Point{1, 1});
  slopes.emplace_back(Point{3, 1});
  slopes.emplace_back(Point{5, 1});
  slopes.emplace_back(Point{7, 1});
  slopes.emplace_back(Point{1, 2});
  std::cout << "Part 2: " << count_trees(slopes, trees, limits) << "\n";
}