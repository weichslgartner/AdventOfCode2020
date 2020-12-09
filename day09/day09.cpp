#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <fmt/printf.h>
#include <deque>
#include <numeric>



std::vector<unsigned long long> parse_input(char const *const file_name) {
	std::ifstream infile(file_name);
	if (!infile.is_open()) {
		std::cerr << "Cannot open " << file_name << "\n";
		return {};
	}
	std::string line;
	std::stringstream ss;
	ss << infile.rdbuf();
	std::vector<unsigned long long> numbers;
	while (std::getline(ss, line)) {
		//fmt::print("{}\n",std::stoi(line));
		numbers.push_back(std::stoull(line));
	}
	return numbers;
}

int main() {
	auto const *const file_name = "build/input/input_09.txt";
	auto numbers = parse_input(file_name);
	std::deque<unsigned long long> deq;
	std::unordered_set<unsigned long long> num_mem;
	constexpr auto prelog_size { 25U };
	auto target { 0ULL };
	auto part2 { 0ULL };
	for (auto const number : numbers) {
		if (deq.size() < prelog_size) {
			deq.push_back(number);
			num_mem.insert(number);
		} else {
			auto found { false };
			for (auto const check : deq) {
				auto need = number - check;
				if (num_mem.contains(need)) {
					found = true;
					break;
				}
			}
			if (not found) {
				//fmt::print("not found {}\n", number);
				target = number;
				break;
			}

			num_mem.erase(deq.front());
			num_mem.insert(number);
			deq.pop_front();
			deq.push_back(number);
		}
	}
	std::vector<unsigned long long> window;
	window.reserve(prelog_size);
	for (int i { 0 }; i < numbers.size(); ++i) {
		auto j { i };
		while (std::accumulate(window.begin(), window.end(), 0ULL) < target and j < numbers.size()) {
			window.push_back(numbers[j]);
			++j;
		}
		if (std::accumulate(window.begin(), window.end(), 0ULL) == target) {
			std::sort(window.begin(), window.end());
			part2 = window.front() + window.back();
			break;
		} else {
			window.clear();
		}
	}
	fmt::print("Part 1: {}\n", target);
	fmt::print("Part 2: {}\n", part2);
	return EXIT_SUCCESS;
}
