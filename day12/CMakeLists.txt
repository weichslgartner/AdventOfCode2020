cmake_minimum_required(VERSION 3.10)
# set the project name
project(day12)
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day12 ${SOURCES})
target_link_libraries(day12 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day12 PRIVATE ../common/)