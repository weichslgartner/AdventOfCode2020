#include "common.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <fstream>
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
	int max;

	auto operator<=>(const ValidRange&) const = default;
};

static auto parse_input(char const *file_name) {
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
		if (line.empty()) {
			nl_cnt++;
			continue;
		}
		std::string token;
		std::stringstream convert { line };
		while (convert >> token) {
			if (std::isdigit(token[0]) and nl_cnt == 0) {
				auto v_range = AOC::parse_number_line<int>(token, '-');
				rules.emplace_back( ValidRange { v_range[0], v_range[1] });
				//fmt::print("{}--{}\n", v_range[0], v_range[1]);
			} else if (std::isdigit(token[0]) and nl_cnt == 1) {
				your_ticket = AOC::parse_number_line<int>(token, ',');
			} else if (std::isdigit(token[0]) and nl_cnt == 2) {
				auto tickets = AOC::parse_number_line<int>(token, ',');
				nearby_ticket.push_back(tickets);
			}

		}
	}
	return std::make_tuple(rules, your_ticket, nearby_ticket);
}

bool eval_rule(int number, auto const &rule) {
	return number >= rule.min && number <= rule.max;
}

auto find_calid_tickets_and_error_rate(auto const &nearby_tickets, auto const &rules){
	std::vector<std::vector<int>> valid_tickets { };
	auto error_rate { 0U };
	for (auto const ticket : nearby_tickets) {
		auto isTicketValid { true };
		for (auto number : ticket) {
			bool isFieldValid { false };
			for (auto const &rule : rules) {
				if (eval_rule(number, rule)) {
					isFieldValid = true;
					break;
				}
				if (number < rule.min) {
					isFieldValid = false;
					break;
				}
			}
			if (not isFieldValid) {
				error_rate += number;
				isTicketValid = false;
				break;
			}
		}
		if (isTicketValid) {
			valid_tickets.push_back(ticket);
		}

	}
	return std::make_tuple(valid_tickets,error_rate);
}

int main() {
	constexpr auto file_name = "build/input/input_16.txt";
	auto [rules, your_ticket, nearby_tickets] = parse_input(file_name);
	auto all_rules = rules;
	std::sort(rules.begin(), rules.end());
	auto[valid_tickets,error_rate]=find_calid_tickets_and_error_rate(nearby_tickets,rules);
	fmt::print("Part 1: {}\n", error_rate);
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

	std::unordered_map<int, std::unordered_set<int>> unique_ids;

	for (auto el : index_map) {
		//fmt::print("{}: ", el.first);
		//auto unique_ids = el.second;
		//std::sort(el.second.begin(),el.second.end());
		for (auto const occ : el.second) {

			if (occ.second == valid_tickets.size()) {
				//	unique_ids.insert(el.first);
				//fmt::print("{},{} size {}| ", occ.first, occ.second,el.second.size());
				if (not unique_ids.contains(el.first)) {
					unique_ids[el.first] = std::unordered_set<int>();
				}
				unique_ids[el.first].insert(occ.first);
			} else {
				//el.second.erase(occ.first);
			}

		}


	}
	auto part2 { 1ULL };
	auto found { 0 };
	while (found < 20) {
		for (auto &el : unique_ids) {
			if (el.second.size() == 1) {
				auto const to_erase = *el.second.begin();
				//fmt::print("{}->{}\n ", el.first,to_erase);
				if (to_erase <= 10) {
					part2 *= static_cast<unsigned long long>(your_ticket.at(el.first));
				}
				found++;
				for (auto &u : unique_ids) {
					u.second.erase(to_erase);
				}
			}
		}
	}

	fmt::print("Part 2: {} \n", part2);

	return EXIT_SUCCESS;
}
