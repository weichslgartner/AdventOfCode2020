cmake_minimum_required(VERSION 3.10)
# set the project name
project(day17)
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day17 ${SOURCES})
target_link_libraries(day17 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day17 PRIVATE ../common/)
