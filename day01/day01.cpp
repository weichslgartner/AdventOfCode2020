#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <string_view>
#include <optional>
#include <fmt/printf.h>


std::unordered_set<long long> parse_list(std::string const & file_name){
    std::unordered_set<long long> numbers;
    std::ifstream infile(file_name);
    std::stringstream ss{};
    std::string token;
    if(infile.is_open()){
        ss << infile.rdbuf();
        while(ss >> token){
            numbers.insert(static_cast<long long>(std::stoi(token)));
        }
    } else {
      fmt::print(stderr, "Cannot open File {}!", file_name);
    }
    return numbers;
}

std::optional<long long> part1(std::unordered_set<long long> &numbers, long long sum){
    for(auto numb  : numbers){
        auto need = sum - numb;
        if(numbers.contains(need)){
            return need*numb;
        }
    }
    return {};
}

std::optional<long long> part2(std::unordered_set<long long> &numbers, long long sum){
    for(auto numb1  : numbers){
        for(auto numb2 : numbers){
            auto need = sum - numb1 - numb2;
            if(numbers.contains(need)){ //numbers.find(need) != numbers.end()
                 return need*numb1*numb2;
            }
        }
    }
    return {};
}

int main(int argc, char **argv) {
  auto const *file_name = "build/input/input_01.txt";
  if (argc == 2) {
    file_name = argv[1];
  }
  auto numbers = parse_list(file_name);
  auto res_part1 = part1(numbers, 2020);
  if (res_part1) {
    fmt::print("Part_1: {}\n", res_part1.value());
  }
  auto res_part2 = part2(numbers, 2020);
  if (res_part1) {
    fmt::print("Part_2: {}\n", res_part2.value());
  }
  return EXIT_SUCCESS;
}