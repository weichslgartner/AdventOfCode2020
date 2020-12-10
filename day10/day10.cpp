#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <fmt/printf.h>
#include <unordered_map>

template<class T>
concept Integral = std::is_integral<T>::value;

template<Integral T>
std::vector<T> parse_input(char const *const file_name) {
	std::ifstream infile(file_name);
	if (!infile.is_open()) {
		fmt::print(stderr, "Cannot open {}\n", file_name);
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

unsigned long long dfs(int const node, int const max, std::unordered_map<int, std::vector<int>> const &dag,
		std::unordered_map<int, unsigned long long> &paths) {
	if (node == max) {
		return 1;
	} else {
		if (not paths.contains(node)) {
			auto sum { 0ULL };
			for (auto const el : dag.at(node)) {
				sum += dfs(el, max, dag, paths);
			}
			paths[node] = sum;
		}
		return paths[node];
	}
}

auto part_1(int const max, std::unordered_set<int> const &set) {
	auto cur_jolt { 0 };
	auto one_diff { 0 };
	auto three_diff { 0 };
	while (cur_jolt < max) {
		if (set.contains(cur_jolt + 1)) {
			cur_jolt++;
			one_diff++;
		} else if (set.contains(cur_jolt + 3)) {
			cur_jolt += 3;
			three_diff++;
		}
	}
	return one_diff * three_diff;
}

std::unordered_map<int, std::vector<int> > parse_dag(std::vector<int> const &numbers, std::unordered_set<int> const &set) {
	std::unordered_map<int, std::vector<int> > dag { };
	for (auto jolt : numbers) {
		if (set.contains(jolt + 1)) {
			if (!dag.contains(jolt)) {
				dag[jolt] = std::vector<int>();
			}
			dag[jolt].push_back(jolt + 1);
		}
		if (set.contains(jolt + 2)) {
			if (!dag.contains(jolt)) {
				dag[jolt] = std::vector<int>();
			}
			dag[jolt].push_back(jolt + 2);
		}
		if (set.contains(jolt + 3)) {
			if (!dag.contains(jolt)) {
				dag[jolt] = std::vector<int>();
			}
			dag[jolt].push_back(jolt + 3);
		}
	}
	return dag;
}

int main() {
	auto const *const file_name = "build/input/input_10.txt";
	using T = int;
	auto numbers = parse_input<T>(file_name);
	auto max = *std::max_element(numbers.begin(), numbers.end());
	numbers.push_back(max + 3);
	numbers.push_back(0);
	std::unordered_set<int> set { };
	set.insert(numbers.begin(), numbers.end());
	auto res1 = part_1(max, set);
	fmt::print("Part 1: {}\n",res1 );
	auto dag = parse_dag(numbers, set);
	std::unordered_map<int, unsigned long long> pathmap { };
	auto paths = dfs(0,max+3,dag,pathmap);
	fmt::print("Part 2: {}\n", paths);
	return EXIT_SUCCESS;
}
