cmake_minimum_required(VERSION 3.10)
# set the project name.
project(day10)
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day10 ${SOURCES})
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(day10 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day10 PRIVATE ../common/)