#include "common.h"
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <deque>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <unordered_set>
#include <vector>

struct ValidRange {
	int min;
	int max;auto operator<=>(const ValidRange&) const = default;
};

inline static std::tuple<std::vector<ValidRange>, std::vector<int>, std::vector<std::vector<int>>> parse_input(char const *file_name) {
	std::ifstream infile { std::string(file_name) };
	if (!infile.is_open()) {
		fmt::print(stderr, "Cannot open {}\n", file_name);
		//return std::make_tuple({},{},{});
	}
	std::string line;
	std::stringstream ss { };
	ss << infile.rdbuf();
	auto nl_cnt { 0U };
	std::vector<ValidRange> rules;
	std::vector<int> your_ticket;
	std::vector<std::vector<int>> nearby_ticket { };
	while (std::getline(ss, line)) {
		//fmt::print("{} {}\n", line, line.size());
		if (line.empty()) {
			nl_cnt++;
			continue;

		}
		ValidRange cur_inst { };
		std::string token;
		std::stringstream convert { line };
		while (convert >> token) {
			if (std::isdigit(token[0]) and nl_cnt == 0) {
				auto v_range = AOC::parse_number_line<int>(token, '-');
				auto r = ValidRange { v_range[0], v_range[1] };
				rules.emplace_back(std::move(r));
				//fmt::print("{}--{}\n", v_range[0], v_range[1]);
			} else if (std::isdigit(token[0]) and nl_cnt == 1) {
				your_ticket = AOC::parse_number_line<int>(token, ',');
			} else if (std::isdigit(token[0]) and nl_cnt == 2) {
				auto tickets = AOC::parse_number_line<int>(token, ',');
				nearby_ticket.push_back(tickets);

			} else {
				//fmt::print("{}\n", token);
			}

		}

	}
	return std::make_tuple(rules, your_ticket, nearby_ticket);
}

int main() {
	constexpr auto file_name = "build/input/input_16.txt";
	auto [rules, your_ticket, nearby_tickets] = parse_input(file_name);
	auto error_rate { 0U };
	auto numb_rules { 12 };
	auto departure_rules = std::vector<ValidRange>(rules.begin(), rules.begin() + numb_rules);
	auto all_rules = rules;
	std::sort(rules.begin(), rules.end());
	auto cnt{0};
	for (auto rule : all_rules) {
		fmt::print("{} {} {}\n",cnt, rule.min, rule.max);
		cnt+=1;
	}
	auto ticket_cnt { 0U };
	auto invalid_cnt { 0U };
	std::vector<std::vector<int>> valid_tickets { };

	for (auto ticket : nearby_tickets) {
		auto i { 0U };
		auto isTicketValid { true };
		for (auto number : ticket) {
			//fmt::print("number {} \n",number);
			bool isFieldValid { false };
			for (auto rule : rules) {
				if (number >= rule.min and number <= rule.max) {
					isFieldValid = true;
					break;
				}
				if (number < rule.min) {
					isFieldValid = false;
					break;
				}
			}
			if (not isFieldValid) {
				//fmt::print("not valid {}\n", number);
				error_rate += number;
				invalid_cnt++;
				isTicketValid = false;
				break;
			}

			i++;

		}
		if (isTicketValid) {
			valid_tickets.push_back(ticket);
			//fmt::print(" valid {}\n", ticket_cnt);
		} else {
			//fmt::print("NOT valid {}\n", ticket_cnt);

		}
		ticket_cnt++;

	}
	std::unordered_map<int, std::unordered_map<int, int>> index_map { };

	for (auto ticket : valid_tickets) {
		auto i { 0U };
		for (auto number : ticket) {
			auto departure_rule_idx_raw { 0U };
			for (auto rule : all_rules) {
				if (number >= rule.min and number <= rule.max) {
					auto departure_rule_idx = departure_rule_idx_raw;
					if (departure_rule_idx_raw % 2 != 0) {
						departure_rule_idx--;
					}

					if (not index_map.contains(i)) {
						index_map.insert( { i, std::unordered_map<int, int>() });
					}
					if (not index_map[i].contains(departure_rule_idx)) {
						index_map[i][departure_rule_idx] = 0;
					}

					index_map[i][departure_rule_idx]++;
				}
				departure_rule_idx_raw++;
			}
			i++;
		}
	}

	fmt::print("Valid tickets: {}\n", valid_tickets.size());
	fmt::print("Part 1: {}\n", error_rate);
	std::unordered_map<int,std::unordered_set<int>> unique_ids;

	for (auto el : index_map) {
		fmt::print("{}: ", el.first);
		//auto unique_ids = el.second;
		//std::sort(el.second.begin(),el.second.end());
		for (auto occ : el.second) {

			if (occ.second == valid_tickets.size()) {
			//	unique_ids.insert(el.first);
				fmt::print("{},{} size {}| ", occ.first, occ.second,el.second.size());
				if(not unique_ids.contains(el.first)){
					unique_ids[el.first] = std::unordered_set<int>();
				}
				unique_ids[el.first].insert( occ.first);
			}else{
				//el.second.erase(occ.first);
			}

		}
		fmt::print("\n");

	}
	auto part2{1ULL};
	auto found{0};
	while(found <20 ){
		auto i{0};
		for (auto &el : unique_ids) {
			if(el.second.size() == 1){
				auto to_erase = *el.second.begin();
				fmt::print("{}->{}\n ", el.first,to_erase);
				if(to_erase <= 10){
					part2 *= static_cast<unsigned long long>(your_ticket.at(el.first));
				}
				found++;
				for (auto &u : unique_ids) {
					u.second.erase(to_erase);
					//fmt::print("erased {}->{}\n ", el.first,to_erase);

				}

			}
			i++;
		}
	}
	fmt::print("your ticket  \n");
	for (auto number : your_ticket) {
		fmt::print(" {} \n", number);
	}

	fmt::print("\n part2 {} \n", part2);
	auto i { 0 };
	for (auto number : your_ticket) {
		auto departure_rule_idx_raw { 0U };
		for (auto rule : departure_rules) {
			if (number >= rule.min and number <= rule.max) {
				auto departure_rule_idx = departure_rule_idx_raw;
				if (departure_rule_idx_raw % 2 != 0) {
					departure_rule_idx--;
				}
				if (unique_ids.contains(i)) {
					//fmt::print("matched {} {}\n", i,departure_rule_idx);
				}
			}
			departure_rule_idx_raw++;
		}
		i++;
	}

	return EXIT_SUCCESS;
}
