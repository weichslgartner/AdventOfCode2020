#include "common.h"
#include <cassert>

#include <fmt/core.h>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstddef>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class OP {
	MULT, ADD, UNDEF
};

struct expr {
	int64_t numb;
	OP op;
};

int64_t solve(std::string_view input) {
	std::vector<expr> stack;
	int64_t res{ 0LL};
	OP last_op { OP::UNDEF };
	for (auto const c : input) {
		if (c == ' ') {
			continue;
		}
		if (std::isdigit(c)) {
			int64_t temp = static_cast<int64_t>(c - '0');
			if (last_op == OP::UNDEF) {
				res = temp;
			} else {
				if (last_op == OP::MULT) {
					res *= temp;
				} else if (last_op == OP::ADD) {
					res += temp;
				}
			}
		} else if (c == '*') {
			last_op = OP::MULT;
		} else if (c == '+') {
			last_op = OP::ADD;
		} else if (c == '(') {
			stack.push_back(expr{res,last_op});
			res = 0LL;
			last_op= OP::UNDEF ;
		} else if (c == ')') {
			auto old_expr = stack.back();
			stack.pop_back();
			if(old_expr.op==OP::MULT){
				res = old_expr.numb*res;
			}else{
				res = old_expr.numb+res;
			}

		}
	}
	return res;
}

void tests() {
	//auto const grid = AOC::parse_grid<char>(file_name);
	auto test = "1 + 2 * 3 + 4 * 5 + 6";
	auto test2 = "2 * 3 + (4 * 5)";
	auto test3 = "5 + (8 * 3 + 9 + 3 * 4 * 3)";
	auto test4 = "5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))";
	auto test5 = "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2";
	assert(solve(test) == 71);
	assert(solve(test2) == 26);
	assert(solve(test3) == 437);
	assert(solve(test4) == 12240);
	assert(solve(test5) == 13632);
}

int main() {
	tests();
	constexpr auto file_name = "build/input/input_18.txt";
	auto const lines = AOC::parse_lines(file_name);
	auto part1{0LL};
	for (auto const &line : lines) {
		part1+= solve(line);
	}
	fmt::print("Part 1: {}\n",part1);
	fmt::print("Part 2: {}\n", 1);
	return EXIT_SUCCESS;
}
