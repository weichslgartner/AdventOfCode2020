#include "common.h"
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

constexpr auto BITMASK_36BIT = 0xFFFFFFFFF;

enum class OP {
	MASK, MEM
};

struct Instruction {
	OP op;
	u_int64_t or_mask;
	u_int64_t and_mask;
	std::string raw_mask;
	u_int64_t address;
	u_int64_t value;
};

inline static std::vector<Instruction> parse_input(char const *file_name) {
	std::ifstream infile { std::string(file_name) };
	if (!infile.is_open()) {
		fmt::print(stderr, "Cannot open {}\n", file_name);
		return {};
	}
	std::string line;
	std::stringstream ss { };
	ss << infile.rdbuf();
	std::vector<Instruction> instructs;
	while (std::getline(ss, line)) {
		Instruction cur_inst { };
		std::string token;
		std::stringstream convert { line };
		convert >> token;
		if (token == "mask") {
			cur_inst.op = OP::MASK;
			convert >> token; //=
			convert >> token;
			auto and_mask = token;
			cur_inst.raw_mask = token;
			std::replace(token.begin(), token.end(), 'X', '0');
			cur_inst.or_mask = static_cast<u_int64_t>(std::stoull(token, nullptr, 2));
			std::replace(and_mask.begin(), and_mask.end(), 'X', '1');
			cur_inst.and_mask = static_cast<u_int64_t>(std::stoull(and_mask, nullptr, 2));
		} else {
			cur_inst.op = OP::MEM;
			cur_inst.address = std::stoull(token.substr(4));
			convert >> token; //=
			convert >> cur_inst.value;
		}
		instructs.push_back(cur_inst);
	}
	return instructs;
}

auto memory_sum(std::unordered_map<u_int64_t, u_int64_t> &memory) {
	return std::accumulate(memory.begin(), memory.end(), 0ULL, [](const auto previous, const auto &element) {
		return previous + element.second;
	});
}

uint64_t part1(std::vector<Instruction> const &instructs, std::unordered_map<u_int64_t, u_int64_t> &memory) {
	uint64_t cur_bitmask_or { };
	uint64_t cur_bitmask_and { };
	for (auto const &inst : instructs) {
		if (inst.op == OP::MASK) {
			cur_bitmask_or = inst.or_mask;
			cur_bitmask_and = inst.and_mask;
		} else {
			memory[inst.address] = (inst.value | cur_bitmask_or) & cur_bitmask_and & BITMASK_36BIT;
		}
	}
	return memory_sum(memory);
}

constexpr uint64_t set_bit(uint64_t dest, uint64_t const position) {
	dest |= 1ULL << position;
	return dest & BITMASK_36BIT;
}

constexpr uint64_t clear_bit(uint64_t dest, uint64_t const position) {
	dest &= ~(1ULL << position);
	return dest & BITMASK_36BIT;
}

uint64_t part2(std::vector<Instruction> const &instructs, std::unordered_map<u_int64_t, u_int64_t> &memory) {
	uint64_t cur_bitmask_or { };
	std::string raw_mask;
	size_t occ { };
	std::vector<uint64_t> positions;
	for (auto const &inst : instructs) {
		if (inst.op == OP::MASK) {
			cur_bitmask_or = inst.or_mask;
			raw_mask = inst.raw_mask;
			occ = std::count(raw_mask.begin(), raw_mask.end(), 'X');
			positions.clear();
			auto pos = raw_mask.find('X');
			while (pos != std::string::npos) {
				positions.push_back(raw_mask.size() - pos);
				pos = raw_mask.find('X', pos + 1);
			}
		} else {
			auto address = inst.address | cur_bitmask_or;
			for (auto bin { 0ULL }; bin < (1ULL << (occ)); ++bin) {
				for (auto i { 0ULL }; i < occ; ++i) {
					auto const shifted = (1 << (occ - 1 - i));
					if (bin & shifted) {
						address = set_bit(address, positions.at(i) - 1);
					} else {
						address = clear_bit(address, positions.at(i) - 1);
					}
				}
				memory[address] = inst.value;
			}
		}
	}
	return memory_sum(memory);
}

int main() {
	constexpr auto file_name = "build/input/input_14.txt";
	auto instructs = parse_input(file_name);
	std::unordered_map<u_int64_t, u_int64_t> memory;
	auto sum = part1(instructs, memory);
	fmt::print("Part 1: {}\n", sum);
	memory.clear();
	sum = part2(instructs, memory);
	fmt::print("Part 2: {}\n", sum);
	return 0;
}
