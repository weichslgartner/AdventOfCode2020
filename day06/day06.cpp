#include <algorithm>
#include <fmt/core.h>
#include <fstream>
#include <span>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
namespace ranges = std::ranges;

std::pair<unsigned, unsigned>
count_answers(std::vector<std::string> const &answers) {
  std::unordered_map<char, unsigned> unique_answers{};
  for (auto const &answer : answers) {
    for (auto const c : answer) {
      ++unique_answers[c];
    }
  }
  return std::make_pair(
      unique_answers.size(),
      ranges::count_if(unique_answers, [&answers](auto const &el) {
        return el.second == answers.size();
      }));
}

int main(int argc, char **argv) {
  auto const *file_name = "build/input/input_06.txt";
  if (argc == 2) {
    std::span<char *> args{argv, 2U};
    file_name = args[1];
  }
  std::ifstream infile(file_name);
  if (not infile.is_open()) {
	fmt::print(stderr,"Cannot open {}\n", file_name);
    return EXIT_FAILURE;
  }
  std::string line;
  std::stringstream ss{};
  ss << infile.rdbuf();
  ss << "\n\n"; // empty line for last answer
  std::vector<std::string> cur_answer;
  auto count_p1{0U};
  auto count_p2{0U};
  while (std::getline(ss, line)) {
    if (not line.empty()) {
      cur_answer.push_back(line);
    } else if (line.empty()) {
      auto [p1, p2] = count_answers(cur_answer);
      count_p1 += p1;
      count_p2 += p2;
      cur_answer.clear();
    }
  }
  fmt::print("Part 1: {}\n", count_p1);
  fmt::print("Part 2: {}\n", count_p2);
  return EXIT_SUCCESS;
}
