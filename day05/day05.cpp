#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <vector>

int find_seat(std::string const &row, int max, char upper, char lower) {
  int min{0};
  for (auto const c : row) {

    if (upper == c)
      min += (max - min) / 2 + 1;

    if (lower == c)
      max -= (max - min) / 2 + 1;
  }
  return max;
}

int get_seat(std::string const &input) {

  auto row = input.substr(0, 7);
  auto column = input.substr(7, 3);
  char upper{'B'};
  char lower{'F'};
  auto row_pos = find_seat(row, 127, upper, lower);
  auto column_pos = find_seat(column, 7, 'R', 'L');
  // std::cout << row_pos << " "<< column_pos;
  return row_pos * 8 + column_pos;
}

void tests() {
  assert(get_seat("FBFBBFFRLR") == 357);
  assert(get_seat("BFFFBBFRRR") == 567);
  assert(get_seat("FFFBBBFRRR") == 119);
  assert(get_seat("BBFFBBFRLL") == 820);
}

int main(int argc, char **argv) {
  tests();
  auto const *file_name = "build/input/input_05.txt";
  if (argc == 2) {
    std::span<char *> args{argv, 2};
    file_name = args[1];
  }
  std::ifstream infile(file_name);
  if (not infile.is_open()) {
    std::cerr << "Cannot open " << file_name << "\n";
    return EXIT_FAILURE;
  }
  std::string line;
  std::vector<int> seats;

  int max = 0;

  while (std::getline(infile, line)) {
    auto id = get_seat(line);
    if (id > max)
      max = id;
    seats.push_back(id);
  }
  std::sort(seats.begin(), seats.end());
  auto last_seat{seats[0] - 1};
  int myseat{0};
  for (auto seat : seats) {
    if ((seat - last_seat) > 1) {
      myseat = seat - 1;
      break;
    }
    last_seat = seat;
  }
  std::cout << "Part 1: " << max << "\n";
  std::cout << "Part 2: " << myseat << "\n";
  return 0;
}
