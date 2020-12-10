#include <algorithm>
#include <assert.h>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>
#include <fmt/printf.h>

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
/*
auto bfs(void max, std::unordered_map<int, std::vector<int> > const &tree) {
	std::deque<int> deq { };
	deq.push_back(0);
	auto paths { 0ULL };
	while (!deq.empty()) {
		auto front = deq.front();
		deq.pop_front();
		if (front == max + 3) {
			paths++;
		} else {
			//fmt::print("{}\n",tree[front].size());
			deq.insert(deq.end(), tree[front].begin(), tree[front].end());
		}
	}
	return paths;
}
*/
unsigned long long dfs(int node, int max, std::unordered_map<int, std::vector<int>>  &dag, std::unordered_map<int,unsigned long long>  &paths) {
	if (node == max) {
		return 1;
	} else {
		if (not paths.contains(node)) {

			auto sum { 0ULL };
			for (auto const el : dag[node]) {
				sum += dfs(el, max, dag, paths);
			}
			paths[node] = sum;
		}
		return paths[node];
	}
}

int main() {
	auto const *const file_name = "build/input/input_10.txt";
	using T = int;
	auto numbers = parse_input<T>(file_name);
	std::unordered_set<int> set { };
	set.insert(numbers.begin(), numbers.end());
	auto max = *std::max_element(numbers.begin(), numbers.end());
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
	three_diff++;
	fmt::print("Part 1: {}\n", one_diff * three_diff);
	std::unordered_map<int, std::vector<int>> dag { };
	numbers.push_back(max + 3);
	numbers.push_back(0);
	set.insert(max + 3);
	std::sort(numbers.begin(), numbers.end());
	for (auto jolt : numbers) {
		if (set.contains(jolt + 1)) {
			if (not dag.contains(jolt)) {
				dag[jolt] = std::vector<int>();
			}
			dag[jolt].push_back(jolt + 1);
		}
		if (set.contains(jolt + 2)) {
			if (not dag.contains(jolt)) {
				dag[jolt] = std::vector<int>();
			}
			dag[jolt].push_back(jolt + 2);
		}
		if (set.contains(jolt + 3)) {
			if (not dag.contains(jolt)) {
				dag[jolt] = std::vector<int>();
			}
			dag[jolt].push_back(jolt + 3);
		}
	}
	//auto paths = bfs(max, tree);
	std::unordered_map<int, unsigned long long> pathmap { };
	auto paths = dfs(0,max+3,dag,pathmap);
	fmt::print("Part 2: {}\n", paths);
	return EXIT_SUCCESS;
}
