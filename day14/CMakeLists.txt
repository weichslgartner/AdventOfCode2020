cmake_minimum_required(VERSION 3.10)
# set the project name
project(day14)
file(GLOB SOURCES *.cpp)
add_executable(day14 ${SOURCES})
target_link_libraries(day14 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day14 PRIVATE ../common/)
