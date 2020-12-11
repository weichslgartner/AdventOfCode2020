cmake_minimum_required(VERSION 3.10)
# set the project name
project(day11)
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day11 ${SOURCES})
target_link_libraries(day11 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day11 PRIVATE ../common/)