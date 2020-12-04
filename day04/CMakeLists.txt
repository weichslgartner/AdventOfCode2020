cmake_minimum_required(VERSION 3.10)

# set the project name
project(day04)

# add the executable
add_executable(day04 day04.cpp)
add_custom_command(
        TARGET day04 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/day04/input*.txt
                ${CMAKE_BINARY_DIR}/input)
