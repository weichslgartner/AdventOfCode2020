#include <fmt/printf.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <span>
#include <sstream>
#include <unordered_set>

using numb_type = unsigned;

std::unordered_set<numb_type> parse_list(std::string const &file_name) {
  std::unordered_set<numb_type> numbers;
  std::ifstream infile(file_name);
  std::stringstream ss{};
  std::string token;
  if (infile.is_open()) {
    ss << infile.rdbuf();
    while (ss >> token) {
      numbers.insert(std::stoul(token));
    }
  } else {
    fmt::print(stderr, "Cannot open File {}!", file_name);
  }
  return numbers;
}

std::optional<unsigned> part1(std::unordered_set<numb_type> &numbers,
                              numb_type sum) {
  for (auto numb : numbers) {
    auto need = sum - numb;
    if (numbers.contains(need)) {
      return need * numb;
    }
  }
  return {};
}

std::optional<numb_type> part2(std::unordered_set<numb_type> &numbers,
                               numb_type sum) {
  for (auto numb1 : numbers) {
    for (auto numb2 : numbers) {
      auto need = sum - numb1 - numb2;
      if (numbers.contains(need)) {
        return need * numb1 * numb2;
      }
    }
  }
  return {};
}

int main(int argc, char **argv) {
  auto const *file_name = "build/input/input_01.txt";
  if (argc == 2) {
    std::span<char *> args{argv, 2};
    file_name = args[1];
  }
  constexpr auto target{2020U};
  auto numbers = parse_list(file_name);
  auto res_part1 = part1(numbers, target);
  if (res_part1) {
    fmt::print("Part_1: {}\n", res_part1.value());
  }
  auto res_part2 = part2(numbers, target);
  if (res_part1) {
    fmt::print("Part_2: {}\n", res_part2.value());
  }
  return EXIT_SUCCESS;
}