#include "common.h"
#include <cassert>
#include <cctype>
#include <cstdint>
#include <fmt/core.h>
#include <string_view>
#include <vector>

enum class OP {
	MULT, ADD, UNDEF
};

struct expr {
	int64_t numb;
	OP op;
	std::vector<int64_t> save_mults;
};

int64_t mult_from_mult_stack(std::vector<int64_t> &save_mults, int64_t res) {
	while (!save_mults.empty()) {
		res *= save_mults.back();
		save_mults.pop_back();
	}
	return res;
}

int64_t solve(std::string_view const input, bool const part2) {
	std::vector<expr> stack;
	std::vector<int64_t> save_mults;
	int64_t res { 0LL };
	OP last_op { OP::UNDEF };
	for (auto const c : input) {
		if (c == ' ') {
			continue;
		}
		if (std::isdigit(c)) {
			int64_t const temp = static_cast<int64_t>(c - '0');
			if (last_op == OP::UNDEF) {
				res = temp;
			} else {
				if (part2) {
					if (last_op == OP::MULT) {
						save_mults.push_back(res);
						res = temp;
					} else if (last_op == OP::ADD) {
						res += temp;
					}
				} else {
					if (last_op == OP::MULT) {
						res *= temp;
					} else if (last_op == OP::ADD) {
						res += temp;
					}
				}
			}
		} else if (c == '*') {
			last_op = OP::MULT;
		} else if (c == '+') {
			last_op = OP::ADD;
		} else if (c == '(') {
			stack.push_back(expr { res, last_op, save_mults });
			save_mults.clear();
			res = 0LL;
			last_op = OP::UNDEF;
		} else if (c == ')') {
			auto const old_expr = stack.back();
			stack.pop_back();
			if (part2) {
				res = mult_from_mult_stack(save_mults, res);
				save_mults.clear();
				if (old_expr.op == OP::MULT) {
					save_mults.push_back(old_expr.numb);
				} else {
					res = old_expr.numb + res;
				}
				save_mults.insert(save_mults.end(), old_expr.save_mults.begin(), old_expr.save_mults.end());
			}
			else {
				if (old_expr.op == OP::MULT) {
					res = old_expr.numb * res;
				} else {
					res = old_expr.numb + res;
				}
			}
		}
	}
	if (part2) {
		res = mult_from_mult_stack(save_mults, res);
	}
	return res;
}

void tests() {
	//auto const grid = AOC::parse_grid<char>(file_name);
	auto test6 = "1 + (2 * 3) + (4 * (5 + 6))";
	auto test = "1 + 2 * 3 + 4 * 5 + 6";
	auto test2 = "2 * 3 + (4 * 5)";
	auto test3 = "5 + (8 * 3 + 9 + 3 * 4 * 3)";
	auto test4 = "5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))";
	auto test5 = "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2";
	auto test7 = "5 + (8 * 3 + 9 + 3 * 4 * 3)";
	auto test8 = "5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))";
	auto test9 = "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2";
	auto test10 = "3 + 9 * 3 + (8+8)";
	auto test11 = "(4 * (5 + 6))";
	assert(solve(test, false) == 71);
	assert(solve(test2, false) == 26);
	assert(solve(test3, false) == 437);
	assert(solve(test4, false) == 12240);
	assert(solve(test5, false) == 13632);
	assert(solve(test, true) == 231);
	assert(solve(test6, true) == 51);
	assert(solve(test7, true) == 1445);
	assert(solve(test7, true) == 1445);
	assert(solve(test8, true) == 669060);
	assert(solve(test9, true) == 23340);
	assert(solve(test10, true) == 228);
	assert(solve(test11, true) == 44);
}

auto solve_and_sum(auto const &lines, bool part2) {
	auto part1 { 0LL };
	for (auto const &line : lines) {
		part1 += solve(line, part2);
	}
	return part1;
}

int main() {
	tests();
	constexpr auto file_name = "build/input/input_18.txt";
	auto const lines = AOC::parse_lines(file_name);
	auto part1 = solve_and_sum(lines, false);
	fmt::print("Part 1: {}\n", part1);
	auto part2 = solve_and_sum(lines, true);
	fmt::print("Part 2: {}\n", part2);
	return EXIT_SUCCESS;
}
