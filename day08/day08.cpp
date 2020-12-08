#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <optional>
#include <algorithm>
#include <unordered_set>
#include <fmt/printf.h>
#include <utility>

enum class Op {
	ACC, JMP, NOP
};

struct Instruction {
	Op op;
	int value;
	bool operator ==(Instruction &rhs) const {
		return op == rhs.op and value == rhs.value;
	}
};

Instruction parse_input_line(std::string_view in) {
	std::string token;
	std::stringstream ss;
	ss << in;
	ss >> token;
	Instruction inst;
	if (token == "jmp") {
		inst.op = Op::JMP;
	} else if (token == "acc") {
		inst.op = Op::ACC;
	} else if (token == "nop") {
		inst.op = Op::NOP;
	} else {
		fmt::print(stderr, "unkown instruction {}\n", token);
	}
	ss >> token;
	inst.value = std::stoi(token);
	return inst;
}

std::vector<Instruction> parse_input(char const *const file_name) {
	std::ifstream infile(file_name);
	if (!infile.is_open()) {
		std::cerr << "Cannot open " << file_name << "\n";
		return {};
	}
	std::string line;
	std::stringstream ss;
	ss << infile.rdbuf();
	std::vector<Instruction> inst_mem;
	while (std::getline(ss, line)) {
		auto inst = parse_input_line(line);
		inst_mem.push_back(std::move(inst));
	}
	return inst_mem;
}


std::optional<int> interprete(std::vector<Instruction> &inst_mem,bool part_1,unsigned max_count) {
	std::unordered_set<int> inst_hist;
	int inst_pnt {0};
	int acc_value {0};
	auto inst_count {0U};
	while (inst_pnt < inst_mem.size()) {
		auto inst = inst_mem.at(inst_pnt);
		if (inst_hist.contains(inst_pnt) and part_1) {
			return acc_value;
		}
		if(inst_count > max_count) {
			return {};
		}
		inst_count++;
		inst_hist.insert(inst_pnt);
		switch (inst.op) {
			case (Op::JMP):
			inst_pnt += inst.value;
			break;
			case (Op::ACC):
			acc_value += inst.value;
			inst_pnt++;
			break;
			case (Op::NOP):
			inst_pnt++;
			break;
			default:
			break;
		}
	}
	return acc_value;
}



std::optional<int> repair(std::vector<Instruction> &inst_mem, unsigned max_count) {
	auto i{0U};
	for (auto &inst : inst_mem) {
		auto const old_inst = inst;
		if (inst.op == Op::NOP) {
			if(inst.value == 0){
				continue;
			}
			inst.op=Op::JMP;
		} else if (inst.op == Op::JMP) {
			inst.op=Op::NOP;
		}else{
			continue;
		}
		auto result = interprete(inst_mem, false, max_count);
		if(result){
			//fmt::print("{}\n",i);
			return result.value();
		}else{
			inst = old_inst;
		}
		i++;
	}
	return {};
}


int repair_search(std::vector<Instruction> &inst_mem){
	auto max_count{100U};
	auto result { repair(inst_mem, max_count)};
	while(not result){
		max_count*=2;
		result = repair(inst_mem, max_count);
	}
	return result.value();
}

void tests() {
	Instruction
	inst {.op = Op::JMP, .value = 3};
	assert(parse_input_line("jmp +3") == inst);
	inst.op = Op::ACC;
	inst.value = -3;
	assert(parse_input_line("acc -3") == inst);
	inst.op = Op::NOP;
	inst.value = 0;
	assert(parse_input_line("nop +0") == inst);
}


int main() {
	tests();
	auto const *const file_name = "build/input/input_08.txt";
	std::vector<Instruction> inst_mem = parse_input(file_name);
	fmt::print("Part 1: {}\n",interprete(inst_mem,true,1000U).value());
	fmt::print("Part 2: {}\n",repair_search(inst_mem));
	return EXIT_SUCCESS;
}
