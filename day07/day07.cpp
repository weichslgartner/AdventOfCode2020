#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fmt/printf.h>
namespace ranges = std::ranges;

struct Bag {
	std::string name;
	int quant;
};
using bag_map_type = std::unordered_map<std::string, std::vector<Bag>>;

void parse_line(std::string_view input, bag_map_type &bag_store) {
	std::stringstream ss { };
	ss << input;
	std::string token { };
	std::vector<std::string> tokenz { };
	bool is_key { true };
	std::string key { };
	std::vector<Bag> value { };
	auto i { 0U };
	while (ss >> token) {
		if (token.find("bag") != std::string::npos) {
			auto bag_name = tokenz[i - 2] + tokenz[i - 1];
			if (is_key) {
				key = bag_name;
				is_key = false;
			} else {
				auto quant { 0 };
				if (tokenz[i - 2] != "no") {
					quant = std::stoi(tokenz[i - 3]);
				}
				value.emplace_back(Bag { bag_name, quant });
			}
		} else {
			tokenz.push_back(token);
			++i;
		}
	}
	bag_store.insert( { key, value });
}

unsigned part_2(bag_map_type &bag_store) {
	std::vector<Bag> vec;
	auto goldbag = bag_store["shinygold"];
	auto count_p2 { 0U };
	vec.insert(vec.begin(), goldbag.begin(), goldbag.end());
	while (!vec.empty()) {
		auto const front = vec.at(0);
		count_p2 += front.quant;
		auto replace = bag_store[front.name];
		ranges::transform(replace, replace.begin(), [&front](auto &x) {
			x.quant *= front.quant;
			return x;
		});
		ranges::copy_if(replace, std::back_inserter(vec), [](auto const &x) {
			return x.quant > 0;
		});
		vec.erase(vec.begin());
	}
	return count_p2;
}

unsigned part_1(bag_map_type &bag_store) {
	auto count_p1 { 0U };
	for (auto const &val : bag_store) {
		auto [key, value] = val;
		while (not value.empty()) {
			auto front = value.at(0);
			if (front.name == "shinygold") {
				count_p1++;
				break;
			}
			if (bag_store.contains(front.name)) {
				auto const replace = bag_store[front.name];
				ranges::copy_if(replace, std::back_inserter(value), [](auto &x) {
					return x.quant > 0;
				});
			}
			value.erase(value.begin());
		}
	}
	return count_p1;
}

int main(int argc, char **argv) {
	//tests();
	auto const *const file_name = "build/input/input_07.txt";
	std::ifstream infile(file_name);
	if (not infile.is_open()) {
		std::cerr << "Cannot open " << file_name << "\n";
		return EXIT_FAILURE;
	}
	std::string line;
	std::stringstream ss;
	ss << infile.rdbuf();
	bag_map_type bag_store { };
	while (std::getline(ss, line)) {
		parse_line(line, bag_store);
	}
	auto const count_p1 { part_1(bag_store) };
	auto const count_p2 { part_2(bag_store) };
	std::cout << "Part 1: " << count_p1 << "\n";
	std::cout << "Part 2: " << count_p2 << "\n";
	return 0;
}
