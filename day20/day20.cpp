#include "common.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <numeric>
#include <fmt/core.h>
#include <functional>
#include <iterator>
#include <string_view>
#include <vector>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
namespace ranges = std::ranges;

class Grid {
public:
	std::vector<std::string> data;
	std::string left_border;
	std::string right_border;
	std::string top_border;
	std::string bottom_border;
	unsigned id;
	unsigned transform_state;
	Grid() : data {  }, id { 0U }, transform_state { 0U } {}
	Grid(std::vector<std::string> const &grid, unsigned const id_) :
			data { grid }, id { id_ }, transform_state { 0U } {
		set_borders();
	}

	Grid(std::vector<std::string> &&grid, unsigned const id_) :
			data { std::move(grid) }, id { id_ }, transform_state { 0U } {
		set_borders();
	}

	void rotate() {
		auto N = data.size();
		for (auto y { 0U }; y < N - 1; ++y) {
			for (auto x { y + 1 }; x < N; ++x) {
				std::swap(data[y][x], data[x][y]);
			}
		}

		for (auto y { 0U }; y < N; ++y) {
			for (auto x { 0U }; x < N / 2; ++x) {
				std::swap(data[y][x], data[y][N - x - 1]);
			}
		}
		set_borders();
	}

	void set_borders() {
		left_border.clear();
		right_border.clear();
		for (auto &line : data) {
			left_border.push_back(line[0]);
			right_border.push_back(line.back());
		}
		top_border = data.front();
		bottom_border = data.back();
	}

	void print() const {
		for (auto const &line : data) {
			fmt::print("{}\n", line);
		}
	}

	void flip() {
		left_border.clear();
		right_border.clear();
		for (auto &line : data) {
			std::reverse(line.begin(), line.end());
			left_border.push_back(line[0]);
			right_border.push_back(line.back());
		}
		top_border = data.front();
		bottom_border = data.back();
	}

	void next_transform() {
		if (transform_state % 4 != 0) {
			rotate();
		} else {
			flip();
		}
		transform_state++;
	}

	void erase_borders(){
		data.erase(data.begin());
		data.pop_back();
		for(std::string &line :data){
			line.erase(line.begin());
			line.pop_back();

		}
	}

	void append_right(std::vector<std::string> &other){
		for(auto i{0U}; i< other.size();++i){
			data[data.size()-other.size()+i] += other[i];
		}
	}

	void append_under(std::vector<std::string> &other){
		data.insert(data.end(), other.begin(), other.end());
	}


	auto size() const{
		return data.size();
	}

	auto count_sharp(){
		//return std::accumulate(data.begin(), data.end(), 0, [](auto const &s){return;});
		//auto cnt_in_string = [](auto const &s){return std::count(s.begin(),s.end(),'#');};
		auto cnt{0U};
		for(auto const &s :data){
			cnt += std::count(s.begin(),s.end(),'#');
		}
		return  cnt;
	}

	bool match(Grid &monster, Point start){

		if(start.y+monster.size() > data.size()){
			return false;
		}
		if(start.x + monster.data.front().size() > data.front().size()){
			return false;
		}
		auto y{start.y};
		for(auto &monster_line : monster.data){
			auto x{start.x};
			for(auto &monster_char : monster_line){
				if(monster_char=='#' and data[y][x] != '#'){
					return false;
				}
				x++;
			}
			y++;
		}
		return true;

	}

	auto cnt_matches(Grid &monster){
		auto cnt{0U};
		for(auto y{0}; y< size() - monster.data.size();++y){
			for(auto x{0}; x< size() - monster.data.front().size();++x){
				if(match(monster, Point{x,y})){
					cnt++;
				}
			}
		}
		return cnt;
	}


};

#ifndef NDEBUG
void tests() {
	std::vector<std::string> test1;
	test1.push_back("012");
	test1.push_back("345");
	test1.push_back("678");
	Grid g { test1, 0 };
	g.print();
	g.next_transform();
	g.print();
	g.next_transform();
	g.print();

}
#endif

auto parse_input(std::vector<std::string> const &lines) {
	std::stringstream ss;
	std::string token;
	unsigned cur_id { };
	auto line_cnt { 0U };
	std::string left_border { };
	std::string right_border { };
	std::string buttom_border { };
	std::string top_border { };
	std::vector<std::string> current_grid { };
	std::unordered_map<unsigned, std::vector<std::string> > id2edges;
	std::unordered_map<std::string, std::vector<unsigned> > edges2id;
	std::unordered_map<unsigned, Grid> id2grid;
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
			id2grid.insert( { cur_id, Grid(current_grid, cur_id) });
			current_grid.clear();
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
			current_grid.push_back(line);
			line_cnt++;
		}
	}

	return std::make_tuple(id2edges, edges2id, id2grid);
}

auto part1(auto const &id2edges, auto const &edges2id) {
	auto result { 1ULL };
	std::vector<int> corner_tiles;
	for (auto const &element : id2edges) {
		auto id = element.first;
		fmt::print("\nid {}", id);
		auto unique_edges { 0U };
		for (auto const &edge : element.second) {
			fmt::print(" {} ", edges2id.at(edge).size());
			if (edges2id.at(edge).size() == 1)
				unique_edges++;
		}
		fmt::print("ue {} ", unique_edges);
		if (unique_edges == 4) {
			result *= static_cast<uint64_t>(id);
			corner_tiles.push_back(id);
		}
	}
	fmt::print("\n");
	return corner_tiles;
}

bool is_unique_border(std::string const &border, std::unordered_map<std::string, std::vector<unsigned> > edges2id) {
	auto edges = edges2id.at(border);
	auto size = edges.size();
	return size == 1U;
}

unsigned get_next_id(std::string const &border, auto &edges2id, unsigned cur_id) {
	auto next_ids = edges2id[border];
	next_ids.erase(std::remove(next_ids.begin(), next_ids.end(), cur_id), next_ids.end());
	fmt::print("next id {} {}\n", next_ids.front(), next_ids.size());
	return next_ids.front();
}

int main() {
	//tests();
	constexpr auto file_name = "build/input/input_20.txt";
	constexpr auto monster_file = "build/input/input_20_monster.txt";
	auto lines = AOC::parse_lines(file_name);
	auto monster = AOC::parse_lines(monster_file);
	//lines.push_back("");
	auto [id2edges, edges2id, id2grid] = parse_input(lines);
	auto corners = part1(id2edges, edges2id);
	fmt::print("Part 1: {}\n", std::accumulate(corners.begin(), corners.end(), 1ULL, std::multiplies<uint64_t>()));
	/*
	std::transform(id2grid.begin(), id2grid.end(), std::back_inserter(all_ids), [](auto const &x) {
		return x.first;
	});*/
	auto supgrid_size = static_cast<unsigned>(std::sqrt(id2edges.size()));
	fmt::print("Tiles: {}\n", supgrid_size);
	std::vector<std::vector<Grid>> supergrid;
	for (auto corner : corners ) { // { 1951 }
		supergrid.clear();
		//std::unordered_set<unsigned> available_corners { corners.begin(), corners.end() };
		/*std::unordered_set<unsigned> available_grids { all_ids.begin(), all_ids.end() };
		std::erase_if(available_grids, [&available_corners](auto const &x) {
			return available_corners.contains(x);
		});
		available_corners.erase(corner);
		*/
		Grid curgrid = id2grid[corner];

		for (auto y { 0U }; y < supgrid_size; ++y) {
			std::vector<Grid> super_grid_line;
			for (auto x { 0U }; x < supgrid_size; ++x) {
				if (y == 0 and x == 0) {
					while ((not is_unique_border(curgrid.top_border, edges2id)) or (not is_unique_border(curgrid.left_border, edges2id))) {
						curgrid.next_transform();
					}
					curgrid.print();
					super_grid_line.push_back(curgrid);
					curgrid = id2grid[get_next_id(curgrid.right_border, edges2id, curgrid.id)];

					// top border
				} else if (y == 0 and x != supgrid_size - 1) {
					fmt::print("top border\n");
					while ((not is_unique_border(curgrid.top_border, edges2id)) or not (curgrid.left_border == super_grid_line[x - 1].right_border)) {
						curgrid.next_transform();
					}
					curgrid.print();
					super_grid_line.push_back(curgrid);
					curgrid = id2grid[get_next_id(curgrid.right_border, edges2id, curgrid.id)];
					// top right corner
				} else if (y == 0 and x == supgrid_size - 1) {
					fmt::print("top right corner\n");
					while (not (is_unique_border(curgrid.top_border, edges2id) and (curgrid.left_border == super_grid_line[x - 1].right_border)
							and is_unique_border(curgrid.right_border, edges2id))) {
						curgrid.next_transform();
					}
					curgrid.print();
					super_grid_line.push_back(curgrid);
					curgrid = id2grid[get_next_id(super_grid_line[0].bottom_border, edges2id, super_grid_line[0].id)];
					// left border
				} else if (y != supgrid_size - 1 and x == 0) {
					fmt::print("left border\n");
					while (not (is_unique_border(curgrid.left_border, edges2id) and (curgrid.top_border == supergrid[y - 1][0].bottom_border))) {
						curgrid.next_transform();
					}
					curgrid.print();
					super_grid_line.push_back(curgrid);
					curgrid = id2grid[get_next_id(curgrid.right_border, edges2id, curgrid.id)];

					// right border
				} else if (y != supgrid_size - 1 and x == supgrid_size - 1) {
					fmt::print("right border\n");
					while (not ((curgrid.left_border == super_grid_line[x - 1].right_border) and is_unique_border(curgrid.right_border, edges2id))) {
						curgrid.next_transform();
					}
					curgrid.print();
					super_grid_line.push_back(curgrid);
					curgrid = id2grid[get_next_id(super_grid_line[0].bottom_border, edges2id, super_grid_line[0].id)];

					// left bottom corner
				} else if (y == supgrid_size - 1 and x == 0) {
					fmt::print("left bottom corner\n");
					while (not ((curgrid.top_border == supergrid[y - 1][x].bottom_border) and is_unique_border(curgrid.left_border, edges2id)
							and is_unique_border(curgrid.bottom_border, edges2id))) {
						curgrid.next_transform();
					}
					curgrid.print();
					super_grid_line.push_back(curgrid);
					curgrid = id2grid[get_next_id(curgrid.right_border, edges2id, curgrid.id)];

					// bottom border
				} else if (y == supgrid_size - 1 and x != supgrid_size - 1) {
					fmt::print("bottom border\n");
					while (not ((curgrid.top_border == supergrid[y - 1][x].bottom_border)
							and (curgrid.left_border == super_grid_line[x - 1].right_border) and is_unique_border(curgrid.bottom_border, edges2id))) {
						curgrid.next_transform();
					}
					curgrid.print();
					super_grid_line.push_back(curgrid);
					curgrid = id2grid[get_next_id(curgrid.right_border, edges2id, curgrid.id)];

					// bottom right corner
				} else if (y == supgrid_size - 1 and x == supgrid_size - 1) {
					fmt::print("bottom right corner\n");
					while (not ((curgrid.top_border == supergrid[y - 1][x].bottom_border)
							and (curgrid.left_border == super_grid_line[x - 1].right_border) and is_unique_border(curgrid.bottom_border, edges2id) and is_unique_border(curgrid.right_border, edges2id))) {
						curgrid.next_transform();
					}
					curgrid.print();
					super_grid_line.push_back(curgrid);
					// inner
				} else {
					fmt::print("inner\n");
					while (not ((curgrid.left_border == super_grid_line[x - 1].right_border)
							and (curgrid.top_border == supergrid[y - 1][x].bottom_border))) {
						curgrid.next_transform();
					}
					curgrid.print();
					super_grid_line.push_back(curgrid);
					curgrid = id2grid[get_next_id(curgrid.right_border, edges2id, curgrid.id)];

				}
			}
			supergrid.push_back(super_grid_line);
		}

	}

	Grid single_grid;
	for(auto &grid_line : supergrid){
		auto i{0U};
		for(Grid &grid : grid_line){
			fmt::print("grid {}\n",grid.id);
			grid.erase_borders();
			grid.print();
			if(i==0U){
				single_grid.append_under(grid.data);

			}else{
				single_grid.append_right(grid.data);
			}
			i++;
		}
	}
	single_grid.print();
	//fmt::print("{}\n",monster[0]);
	std::transform(monster.begin(),monster.end(),monster.begin(),[](auto &s){  std::replace( s.begin(), s.end(), ' ', '.'); return s;});
	Grid monstergrid{monster,666};
	monstergrid.print();
	for (int var = 0; var < 8; ++var) {
		auto matches = single_grid.cnt_matches(monstergrid);
		auto remaining_sharp = single_grid.count_sharp() - matches * monstergrid.count_sharp();
		fmt::print("Matches {} {}\n",matches,remaining_sharp);
		single_grid.next_transform();
	}
	fmt::print("Part 2: {}\n", monstergrid.count_sharp());
	return EXIT_SUCCESS;
}

