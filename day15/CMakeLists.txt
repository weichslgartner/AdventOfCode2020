cmake_minimum_required(VERSION 3.10)
# set the project name
project(day15)
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day15 ${SOURCES})
target_link_libraries(day15 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day15 PRIVATE ../common/)
