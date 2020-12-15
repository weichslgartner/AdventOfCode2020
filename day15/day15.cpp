#include <fmt/core.h>
#include <deque>
#include <unordered_map>

auto solve(std::deque<int> init_numbers, int const bound) {
	std::unordered_map<int, int> memory;
	auto current_number { 0 };
	auto last_number { -1 };
	for (auto i { 1 }; i <= bound; ++i) {
		if (!init_numbers.empty()) {
			current_number = init_numbers.front();
			init_numbers.pop_front();
		} else {
			if (!memory.contains(last_number)) {
				current_number = 0;
			} else {
				auto last_time = memory[last_number];
				current_number = i - 1 - last_time;
			}
		}
		//fmt::print("{}: {}\n", i, current_number);
		if (memory.contains(last_number)) {
			memory.at(last_number) = i - 1;
		} else {
			memory.insert( { last_number, i - 1 });
		}
		last_number = current_number;
	}
	return current_number;
}

int main() {
	std::deque<int> init_numbers { 6, 13, 1, 15, 2, 0 };
	//std::deque<int> init_numbers{0,3,6};
	auto res = solve(init_numbers, 2020);
	fmt::print("Part 1: {}\n", res);
	res = solve(init_numbers, 30000000);
	fmt::print("Part 2: {}\n", res);
	return EXIT_SUCCESS;
}
