cmake_minimum_required(VERSION 3.10)
project(day19)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day19 ${SOURCES})
target_link_libraries(day19 PRIVATE aoc_lib  ${CONAN_LIBS} )
target_include_directories(day19 PRIVATE ../common/)