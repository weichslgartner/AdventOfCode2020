#include "common.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <deque>
#include <fmt/core.h>
#include <vector>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <cstddef>
#include <functional>
#include <utility>
namespace ranges = std::ranges;

void print_cups(unsigned int current_cup_pos, auto const & cups) {
	auto j { 0U };
	for (auto c : cups) {
		if (j == current_cup_pos) {
			fmt::print("({}) ", c);
		} else {
			fmt::print("{} ", c);
		}
		j++;
	}
	fmt::print("\n");

}

void pickup(std::vector<unsigned> const &cups, unsigned int current_cup_pos, unsigned length, std::vector<unsigned> &pick_up) {
	pick_up.clear();
	fmt::print("Pick up: ");
	for (auto j { 1 }; j < 4; j++) {
		auto tmp = cups[(current_cup_pos + j) % length];
		pick_up.push_back(tmp);
		fmt::print("{} ", tmp);
	}
	fmt::print("\n");
}

int main() {
	auto input = std::string{"624397158"};
	std::vector<unsigned> cups;
	std::vector<unsigned> pick_up;
	for (auto i : input) {
		cups.push_back(i-'0');
	}
	auto current_cup_pos { 0U };
	auto current_cup = cups[current_cup_pos];

	auto length = cups.size();
	for (auto i { 1 }; i <= 100; ++i) {
		fmt::print("Move {}\n",i);
		print_cups(current_cup_pos,cups);
		pickup(cups, current_cup_pos, length, pick_up);
		cups.erase(std::remove_if(cups.begin(), cups.end(), [&pick_up] (auto const x){return std::find(pick_up.begin(),pick_up.end(),x) != pick_up.end();}),cups.end());
		//fmt::print("cups size: {}\n", cups.size());
		//print_cups(current_cup_pos,cups);

		auto destination{ current_cup-1};
		while(std::find(pick_up.begin(),pick_up.end(), destination) != pick_up.end() or destination==0){
			if(destination==0){
				destination=length;
			}else{
				destination = (destination-1) % length;
			}
		}
		fmt::print("destination: {}\n", destination);

		auto insert_point = std::find(cups.begin(),cups.end(), destination);
		cups.insert(insert_point+1, pick_up.begin(), pick_up.end());
		auto next_cup = std::find(cups.begin(),cups.end(),current_cup);
		if(next_cup < cups.end()-1){
			current_cup= *(next_cup +1);

		}else{
			current_cup = cups.front();

		}
		current_cup_pos = (current_cup_pos+1) %length;
		while(cups[current_cup_pos] != current_cup){
			std::rotate(cups.begin(),cups.begin()+1,cups.end());
		}

	}
	print_cups(current_cup_pos,cups);
	std::string result{};
	auto one_pos = std::find(cups.begin(),cups.end(),1);
	while(true){
		one_pos++;
		if(one_pos == cups.end()){
			one_pos = cups.begin();
		}
		auto const c = '0' + *one_pos;
		if(c == '1'){
			break;
		}
		result+=c;
	}
	fmt::print("Part 1: {}\n", result);
	fmt::print("Part 2: {}\n", 0);
	return EXIT_SUCCESS;
}

