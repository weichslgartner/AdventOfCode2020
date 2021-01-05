cmake_minimum_required(VERSION 3.10)

# set the project name
project(day06)

# add the executable
add_executable(day06 day06.cpp)
target_link_libraries(day06 PRIVATE aoc_lib  ${CONAN_LIBS} )
