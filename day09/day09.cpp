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

template <class T>
concept Integral = std::is_integral<T>::value;

template<Integral T>
std::vector<T> parse_input(char const *const file_name) {
	std::ifstream infile(file_name);
	if (!infile.is_open()) {
		std::cerr << "Cannot open " << file_name << "\n";
		return {};
	}
	std::string line;
	std::stringstream ss { };
	ss << infile.rdbuf();
	std::vector<T> numbers;
	while (std::getline(ss, line)) {
		T number;
		std::stringstream convert { line };
		convert >> number;
		numbers.push_back(number);
	}
	return numbers;
}
template<Integral T>
auto part_1(std::vector<T> const &numbers, T prelog_size) {
	T target { 0 };
	std::deque<T> deq { };
	std::unordered_set<T> num_mem { };
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
			if (!found) {
				target = number;
				break;
			}
			num_mem.erase(deq.front());
			num_mem.insert(number);
			deq.pop_front();
			deq.push_back(number);
		}
	}
	return target;
}

template<Integral T>
auto part_2(std::vector<T> const &numbers, T target) {
	T part2 { };
	std::vector<T> window { };
	for (int i { 0 }; i < numbers.size(); ++i) {
		auto j { i };
		T sum { };
		while ((sum = std::accumulate(window.begin(), window.end(), T { 0 })) < target && j < numbers.size()) {
			window.push_back(numbers[j]);
			++j;
		}
		if (sum == target) {
			std::sort(window.begin(), window.end());
			part2 = window.front() + window.back();
			break;
		} else {
			window.clear();
		}
	}
	return part2;
}

int main() {
	auto const *const file_name = "build/input/input_09.txt";
	using T = unsigned long long;
	auto numbers = parse_input<T>(file_name);
	constexpr auto prelog_size { 25U };
	auto target = part_1<T>(numbers, prelog_size);
	auto part2 = part_2<T>(numbers, target);
	fmt::print("Part 1: {}\n", target);
	fmt::print("Part 2: {}\n", part2);
	return EXIT_SUCCESS;
}
