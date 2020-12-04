#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool check_haircolor(std::string const &in) {
  constexpr auto hair_len{7};
  if (in.size() != hair_len) {
    return false;
  }
  if (in[0] != '#') {
    return false;
  }
  return std::count_if(in.begin(), in.end(), [](unsigned char c) {
           return std::isxdigit(c);
         }) == hair_len-1;
}

bool check_eyecolor(std::string const &in) {
  if (in.size() != 3) {
    return false;
  }
  std::array<std::string, 7> const valid_ec  {"amb", "blu", "brn", "gry",
                                         "grn", "hzl", "oth"};
  for (auto const &color : valid_ec) {
    if (in.find(color) != std::string::npos) {
      return true;
    }
  }
  return false;
}

bool check_id(std::string const &in) {
  constexpr auto id_len{9};
  if (in.size() != id_len) {
    return false;
  }
  return std::count_if(
             in.begin(), in.end(),
             [](unsigned char c) { return std::isdigit(c); } // correct
             ) == id_len;
}

bool check_year(std::string const &in, int min, int max) {
  if (in.size() != 4) {
    return false;
  }
  auto year{std::stoul(in)};
  return year >= min and year <= max;
}
/*
If cm, the number must be at least 150 and at most 193.
If in, the number must be at least 59 and at most 76.
*/
bool check_height(std::string const &in) {
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

bool test_passport_p1(std::string const &passport) {
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

/**
 * byr (Birth Year) - four digits; at least 1920 and at most 2002.
iyr (Issue Year) - four digits; at least 2010 and at most 2020.
eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
*/
bool test_passport_p2(std::string const &passport) {
  auto byr = passport.find("byr:");
  if (byr != std::string::npos) {
    auto token_end = passport.find_first_of(' ', byr);
    auto byr_year = passport.substr(byr + 4, token_end - (byr + 4));
    if (not check_year(byr_year, 1920, 2002)) {
      return false;
    }
  } else {
    return false;
  }
  auto iyr = passport.find("iyr:");
  if (iyr != std::string::npos) {
    auto token_end = passport.find_first_of(' ', iyr);
    auto iyr_year = passport.substr(iyr + 4, token_end - (iyr + 4));
    if (not check_year(iyr_year, 2010, 2020)) {
      return false;
    }
  } else {
    return false;
  }
  auto eyr = passport.find("eyr:");
  if (eyr != std::string::npos) {
    auto token_end = passport.find_first_of(' ', eyr);
    auto eyr_year = passport.substr(eyr + 4, token_end - (eyr + 4));
    if (not check_year(eyr_year, 2020, 2030)) {
      return false;
    }
  } else {
    return false;
  }
  auto hgt = passport.find("hgt:");
  if (hgt != std::string::npos) {
    auto token_end = passport.find_first_of(' ', hgt);
    auto height = passport.substr(hgt + 4, token_end - (hgt + 4));
    if (not check_height(height)) {
      return false;
    }
  } else {
    return false;
  }
  auto hcl = passport.find("hcl:");
  if (hcl != std::string::npos) {
    auto token_end = passport.find_first_of(' ', hcl);
    auto hair_color = passport.substr(hcl + 4, token_end - (hcl + 4));
    if (not check_haircolor(hair_color)) {
      return false;
    }
  } else {
    return false;
  }
  auto ecl = passport.find("ecl:");
  if (ecl != std::string::npos) {
    auto token_end = passport.find_first_of(' ', ecl);
    auto eye_color = passport.substr(ecl + 4, token_end - (ecl + 4));
    if (not check_eyecolor(eye_color)) {
      return false;
    }
  } else {
    return false;
  }
  auto pid = passport.find("pid:");
  if (pid != std::string::npos) {
    auto token_end = passport.find_first_of(' ', pid);
    auto id = passport.substr(pid + 4, token_end - (pid + 4));
    if (not check_id(id)) {
      return false;
    }
  } else {
    return false;
  }
  return true;
}

void tests() {
  assert(check_height("149cm")==false);
  assert(check_height("190in") == false);
  assert(check_height("190") == false);
  assert(check_haircolor("#123abc"));
  assert(check_haircolor("#123abz") == false);
  assert(check_haircolor("123abc") == false);
  assert(check_eyecolor("wat") == false);
  assert(check_eyecolor("brn"));
  assert(check_id("000000001"));
  assert(check_id("0123456789") == false);
}

int main() {
  tests();
  std::ifstream infile("build/input/input_04.txt");
  std::string line;
  std::stringstream ss;
  ss << infile.rdbuf();
  ss << "\n\n"; //empty line for last rule
  std::string cur_passport;
  std::vector<std::string> passports;
  int c_valid{0};
  while (std::getline(ss, line)) {
    cur_passport.append(line);
    cur_passport.append(" ");
    if (line.empty()) {
    passports.push_back(cur_passport);
    cur_passport.clear();
    }
  }
  namespace ranges = std::ranges;
  std::cout << "Part 1: " << ranges::count_if(passports,[](std::string const &p){return test_passport_p1(p); }) << "\n";
  std::cout << "Part 2: " << ranges::count_if(passports,[](std::string const &p){return test_passport_p2(p); }) << "\n";

  return 0;
}
