cmake_minimum_required(VERSION 3.10)
project(day22)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day22 ${SOURCES})
target_link_libraries(day22 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day22 PRIVATE ../common/)