cmake_minimum_required(VERSION 3.10)
project(day23)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day23 ${SOURCES})
target_link_libraries(day23 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day23 PRIVATE ../common/)