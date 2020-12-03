cmake_minimum_required(VERSION 3.19)

# set the project name
project(day03)

# add the executable
add_executable(day03 day03.cpp)
add_custom_command(
        TARGET day03 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/day03/input*.txt
                ${CMAKE_BINARY_DIR}/input)
