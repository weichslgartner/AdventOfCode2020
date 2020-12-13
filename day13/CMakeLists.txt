cmake_minimum_required(VERSION 3.10)
# set the project name
project(day13)
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day13 ${SOURCES})
target_link_libraries(day13 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day13 PRIVATE ../common/)
