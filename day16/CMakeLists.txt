cmake_minimum_required(VERSION 3.10)
# set the project name
project(day16)
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day16 ${SOURCES})
target_link_libraries(day16 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day16 PRIVATE ../common/)
