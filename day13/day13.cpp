#include "common.h"

#include <algorithm>
#include <numeric>
#include <cassert>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using int_type = unsigned long long;

inline static std::vector<int_type> parse_csv(char const *file_name, std::vector<int_type> &positions) {
	std::ifstream infile { std::string(file_name) };
	if (!infile.is_open()) {
		fmt::print(stderr, "Cannot open {}\n", file_name);
		return {};
	}
	std::string line;
	std::stringstream ss { };
	ss << infile.rdbuf();
	std::vector<int_type> numbers;
	while (std::getline(ss, line)) {
		int_type position { 0 };
		positions.clear();
		unsigned number;
		std::string token;
		std::stringstream convert { line };
		while (std::getline(convert, token, ',')) {
			if (token == "x") {
				position++;
				continue;
			}
			try {
				number = std::stoull(token);

				numbers.push_back(number);
				positions.push_back(position);
				position++;

			} catch (const std::exception &e) {
			}

		}
	}
	return numbers;
}

unsigned part_1(std::vector<int_type> const &numbers, int_type cur_time) {
	int_type min { 500 };
	int_type id;
	for (auto i : numbers) {
		auto wait_time = (cur_time / i + (cur_time % i != 0)) * i - cur_time;
		if (wait_time < min) {
			min = wait_time;
			id = i;
		}
	}
	return id * min;
}

bool check_condition(std::vector<int_type> const &numbers, int_type to_check, std::vector<int_type> &positions) {
	unsigned index { 0U };
	for (auto id : numbers) {
		auto offset = positions[index];
		if ((to_check + offset) % id != 0) {
			//fmt::print("x1+{}%{}\n", offset, id);
			return false;
		}
		index++;
	}
	return true;
}

int_type inverse(int_type a, int_type b){
	auto s{0ULL};
	auto old_s{1ULL};
	auto r{b};
	auto old_r{a};
	while(r!=0){
		auto quot = old_r / r;

	}
	return 0;
}

int_type inv(long long a, long long  b)
{
	long long b0 = b;
	long long		temp { }, quotient;
	long long  x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		quotient = a / b;
		temp = b;
		b = a % b; // remainder
		a = temp;
		temp = x0;
		x0 = x1 - quotient * x0; 
		x1 = temp;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}


int main() {
	std::vector<int_type> positions;
	constexpr auto file_name = "build/input/input_13.txt";
	auto numbers = parse_csv(file_name, positions);
	//print_grid(grid);
	auto cur_time = numbers.front();
	numbers.erase(numbers.begin());
	fmt::print("Part 1: {}\n", part_1(numbers,cur_time));
	//assert(numbers.size()==positions.size());
	auto max_id = std::max_element(numbers.begin(), numbers.end());
	auto idx = max_id - numbers.begin();
	std::vector<int_type> factors;
	unsigned index { 0U };
	for (auto id : numbers) {
		auto offset = positions[index];

		//fmt::print("opt.add((x1+{})%{}==0)\n", offset, id);
		//fmt::print("x = {} mod {}\n",(id*4-offset) %id, id);
		factors.push_back((id*4-offset) %id);
		index++;
	}
	auto M = std::accumulate(numbers.begin(), numbers.end(),  1ULL, std::multiplies<int_type>());
	std::vector<int_type> m;
	//fmt::print("{}\n",M);
	std::transform(numbers.begin(), numbers.end(),std::back_inserter(m),[M](auto x){return M/x;});
	auto result{0ULL};
	for(auto i{0};i<numbers.size();++i){
		auto m_= m[i];
		auto n_ = numbers[i];
		auto res = inv(m_,n_);
		result += factors[i]*res*m[i] ;
		result %= M;
	}
	fmt::print("Part 2: {}\n",result);

	// 760653049054623 too low
	//
	//check_condition(numbers,3417,positions);
/*
	auto start { 202981789065578ULL };
	while ((start + positions[idx]) % *max_id) {
		start++;
	}

	for (int_type i { start };; i += *max_id) {
		if (check_condition(numbers, i, positions)) {

			fmt::print("Part 2: {}\n", i);
			break;
		}
	}
*/
	return EXIT_SUCCESS;
}
