cmake_minimum_required(VERSION 3.10)
project(day21)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day21 ${SOURCES})
target_link_libraries(day21 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day21 PRIVATE ../common/)