#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <vector>
namespace ranges = std::ranges;

constexpr bool check_hcl(std::string const &in) {
  constexpr auto hair_len{7U};
  if ((in.size() != hair_len) or (in[0] != '#')) {
    return false;
  }
  return ranges::count_if(in, [](unsigned char c) {
           return std::isxdigit(c);
         }) == hair_len - 1;
}

constexpr bool check_ecl(std::string const &in) {
  if (in.size() != 3U) {
    return false;
  }
  constexpr std::array const valid_ec  {"amb", "blu", "brn", "gry",
                                         "grn", "hzl", "oth"};
  return ranges::count_if(valid_ec, [in](std::string const &color) {
           return in.find(color) != std::string::npos;
         }) == 1U;
}

constexpr bool check_pid(std::string const &in) {
  constexpr auto id_len{9U};
  if (in.size() != id_len) {
    return false;
  }
  return ranges::count_if(
             in, [](unsigned char c) { return std::isdigit(c); } // correct
             ) == id_len;
}

constexpr bool check_year(std::string const &in, unsigned min, unsigned max) {
  if (in.size() != 4U) {
    return false;
  }
  auto year{std::stoul(in)};
  return year >= min and year <= max;
}

constexpr bool check_hgt(std::string const &in) {
  if (in.find("in") != std::string::npos) {
    auto height{std::stoul(in.substr(0, in.size() - 2))};
    return height >= 59 and height <= 76;
  }
  if (in.find("cm") != std::string::npos) {
    auto height{std::stoul(in.substr(0, in.size() - 2))};
    return height >= 150 and height <= 193;
  }
  return false;
}

constexpr bool check_byr(std::string const &in) { return check_year(in, 1920, 2002); }

constexpr bool check_iyr(std::string const &in) { return check_year(in, 2010, 2020); }

constexpr bool check_eyr(std::string const &in) { return check_year(in, 2020, 2030); }

bool check_field(std::string const &passport, std::string const &key,
                 std::function<bool(std::string const &)> fun) {
  auto value_pos{passport.find(key)};
  auto key_len{key.size()};
  if (value_pos != std::string::npos) {
    auto token_end = passport.find_first_of(' ', value_pos);
    auto token =
        passport.substr(value_pos + key_len, token_end - (value_pos + key_len));
    return fun(token);
  }
  return false;
}

bool test_passport_p2(std::string const &passport) {
  auto byr{check_field(passport, "byr:", check_byr)};
  auto iyr{check_field(passport, "iyr:", check_iyr)};
  auto eyr{check_field(passport, "eyr:", check_eyr)};
  auto hgt{check_field(passport, "hgt:", check_hgt)};
  auto hcl{check_field(passport, "hcl:", check_hcl)};
  auto ecl{check_field(passport, "ecl:", check_ecl)};
  auto pid{check_field(passport, "pid:", check_pid)};
  return byr and iyr and eyr and hgt and hcl and ecl and pid;
}

constexpr bool test_passport_p1(std::string const &passport) {
  auto byr = passport.find("byr:") != std::string::npos;
  auto iyr = passport.find("iyr:") != std::string::npos;
  auto eyr = passport.find("eyr:") != std::string::npos;
  auto hgt = passport.find("hgt:") != std::string::npos;
  auto hcl = passport.find("hcl:") != std::string::npos;
  auto ecl = passport.find("ecl:") != std::string::npos;
  auto pid = passport.find("pid:") != std::string::npos;
  //auto cid = passport.find("cid:") != std::string::npos;
  return byr and iyr and eyr and hgt and hcl and ecl and pid;
}

void tests() {
  assert(check_hgt("149cm") == false);
  assert(check_hgt("190in") == false);
  assert(check_hgt("190") == false);
  assert(check_hcl("#123abc"));
  assert(check_hcl("#123abz") == false);
  assert(check_hcl("123abc") == false);
  assert(check_ecl("wat") == false);
  assert(check_ecl("brn"));
  assert(check_pid("000000001"));
  assert(check_pid("0123456789") == false);
}

int main(int argc, char **argv) {
  tests();
  auto const *file_name = "build/input/input_04.txt";
  if (argc == 2) {
    std::span<char *> args{argv, 2};
    file_name = args[1];
  }
  std::ifstream infile(file_name);
  if(not infile.is_open()){
	  std::cerr << "Cannot open " << file_name << "\n";
	  return EXIT_FAILURE;
  }
  std::string line;
  std::stringstream ss;
  ss << infile.rdbuf();
  ss << "\n\n"; //empty line for last rule
  std::string cur_passport;
  std::vector<std::string> passports;
  while (std::getline(ss, line)) {
    cur_passport.append(line);
    cur_passport.append(" ");
    if (line.empty()) {
    passports.push_back(cur_passport);
    cur_passport.clear();
    }
  }
  std::cout << "Part 1: " << ranges::count_if(passports,[](std::string const &p){return test_passport_p1(p); }) << "\n";
  std::cout << "Part 2: " << ranges::count_if(passports,[](std::string const &p){return test_passport_p2(p); }) << "\n";
  return EXIT_SUCCESS;
}
