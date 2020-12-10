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


template<Integral T>
std::unordered_map<T, std::vector<T> > parse_dag(std::vector<T> const &numbers, std::unordered_set<T> const &set) {
	std::unordered_map<T, std::vector<T> > dag { };
	for (auto jolt : numbers) {
		for (int i { 1 }; i <= 3; ++i) {
			if (set.contains(jolt + i)) {
				if (!dag.contains(jolt)) {
					dag[jolt] = std::vector<T>();
				}
				dag[jolt].push_back(jolt + i);
			}
		}
	}
	return dag;
}

template<Integral T, Integral Big>
Big dfs(T const node, T const max, std::unordered_map<T, std::vector<T>> const &dag,
		std::unordered_map<T, Big> &paths) {
	if (node == max) {
		return Big{1};
	} else {
		if (not paths.contains(node)) {
			Big sum { 0 };
			for (auto const el : dag.at(node)) {
				sum += dfs(el, max, dag, paths);
			}
			paths[node] = sum;
		}
		return paths[node];
	}
}

template<Integral T>
auto part_1(int const max, std::unordered_set<T> const &set) {
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



int main() {
	auto const *const file_name = "build/input/input_10.txt";
	using T = unsigned;
	using Big = unsigned long long;
	auto numbers = parse_input<T>(file_name);
	auto max = *std::max_element(numbers.begin(), numbers.end());
	numbers.push_back(max + 3);
	numbers.push_back(0);
	std::unordered_set<T> set { };
	set.insert(numbers.begin(), numbers.end());
	auto res1 = part_1<T>(max, set);
	fmt::print("Part 1: {}\n", res1);
	auto dag = parse_dag<T>(numbers, set);
	std::unordered_map<T,Big> pathmap { };
	auto paths = dfs(T{0}, max + 3, dag, pathmap);
	fmt::print("Part 2: {}\n", paths);
	return EXIT_SUCCESS;
}
