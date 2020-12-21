cmake_minimum_required(VERSION 3.10)
project(day20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day20 ${SOURCES})
target_link_libraries(day20 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day20 PRIVATE ../common/)