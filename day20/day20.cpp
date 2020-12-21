#include "common.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <deque>
#include <fmt/core.h>
#include <string_view>
#include <vector>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
namespace ranges = std::ranges;
auto parse_input(std::vector<std::string> const &lines) {
	std::stringstream ss;
	std::string token;
	unsigned cur_id { };
	auto line_cnt { 0U };
	std::string left_border { };
	std::string right_border { };
	std::string buttom_border { };
	std::string top_border { };
	std::unordered_map<unsigned, std::vector<std::string> > id2edges;
	std::unordered_map<std::string, std::vector<unsigned> > edges2id;
	for (auto const &line : lines) {
		if (line.find("Tile") != std::string::npos) {
			ss.clear();
			ss << line;
			ss >> token;
			ss >> token;
			cur_id = std::stoul(token);
			//	fmt::print("id {}\n", cur_id);
		} else if (line.empty()) {
			std::vector edges { top_border, buttom_border, left_border, right_border };
			auto reversed_edges = edges;
			ranges::for_each(reversed_edges, [](auto &line) {
				return std::reverse(line.begin(), line.end());
			});
			edges.insert(edges.end(), reversed_edges.begin(), reversed_edges.end());
			for (auto const &edge : edges) {
				if (!edges2id.contains(edge)) {
					edges2id.insert( { edge, std::vector<unsigned>() });

				}
				//	fmt::print("{}-> {}\n",edge, cur_id);
				edges2id[edge].push_back(cur_id);
			}
			id2edges.insert( { cur_id, std::move(edges) });
			line_cnt = 0U;
			left_border = "";
			right_border = "";
		} else {
			if (line_cnt == 0) {
				top_border = line;
			}
			left_border += line[0];
			right_border += line.back();
			buttom_border = line;
			line_cnt++;
		}
	}

	return std::make_pair(id2edges, edges2id);
}

auto part1(auto const &id2edges, auto const &edges2id) {
	auto result { 1ULL };
	for (auto const &element : id2edges) {
		auto id = element.first;
		//	fmt::print("\nid {}",id);
		auto unique_edges { 0U };
		for (auto const &edge : element.second) {
			//fmt::print(" {} ",edges2id[edge].size());
			if (edges2id.at(edge).size() == 1)
				unique_edges++;
		}
		//	fmt::print("ue {} ",unique_edges);
		if (unique_edges == 4) {
			result *= static_cast<uint64_t>(id);
		}
	}
	//fmt::print("\n");
	return result;
}

int main() {
//tests();
	constexpr auto file_name = "build/input/input_20.txt";
	auto lines = AOC::parse_lines(file_name);
	lines.push_back("");
	auto [id2edges, edges2id] = parse_input(lines);
	auto result = part1(id2edges, edges2id);
	fmt::print("Part 1: {}\n", result);
	fmt::print("Part 2: {}\n", 0);
	return EXIT_SUCCESS;
}

