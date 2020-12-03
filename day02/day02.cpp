#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

struct pw_rule {
  uint32_t min_occ;
  uint32_t max_occ;
  char character;
  std::string pw;
};

bool check_part1(pw_rule const &rule) {
  namespace ranges = std::ranges;
  auto count = ranges::count_if(
      rule.pw, [&rule](char i) { return i == rule.character; });
  return count >= rule.min_occ and count <= rule.max_occ;
}

bool check_part2(pw_rule const &rule) {
  return (rule.pw.at(rule.min_occ - 1) == rule.character) xor
         (rule.pw.at(rule.max_occ - 1) == rule.character);
}

std::pair<uint32_t, uint32_t> parse_occurences(std::string const &input) {
  std::stringstream ss{input};
  std::pair<uint32_t, uint32_t> result;
  std::string token;
  std::getline(ss, token, '-');
  result.first = std::stoul(token);
  std::getline(ss, token, '-');
  result.second = std::stoul(token);
  return result;
}

pw_rule parse_line(std::string const &line) {
  std::string occurrences;
  std::string character;
  std::string password;
  std::stringstream ss{line};
  ss >> occurrences;
  auto [min_occ, max_occ] = parse_occurences(occurrences);
  ss >> character;
  character = character.substr(0, 1);
  ss >> password;
  return pw_rule{.min_occ = min_occ,
                 .max_occ = max_occ,
                 .character = character[0],
                 .pw = password};
}

int main(int argc, char **argv) {

  auto const *file_name = "build/input/input_02.txt";
  if (argc == 2) {
    std::span<char *> args{argv, 2};
    file_name = args[1];
  }
  std::ifstream infile(file_name);
  if (not infile.is_open()) {
    std::cerr << "Failed open \n";
    return EXIT_FAILURE;
  }
  std::string line;
  auto count_1{0};
  auto count_2{0};
  if (infile.is_open()) {
    while (std::getline(infile, line)) {
      auto rule = parse_line(line);
      count_1 += static_cast<int>(check_part1(rule));
      count_2 += static_cast<int>(check_part2(rule));
    }
  }
  std::cout << "Part 1: " << count_1 << "\n";
  std::cout << "Part 2: " << count_2 << "\n";
  return EXIT_SUCCESS;
}