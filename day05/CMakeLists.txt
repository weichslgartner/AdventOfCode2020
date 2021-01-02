cmake_minimum_required(VERSION 3.10)
# set the project name
project(day05)
# add the executable
add_executable(day05 day05.cpp)
target_link_libraries(day05 PRIVATE aoc_lib  ${CONAN_LIBS} )
add_custom_command(
        TARGET day05 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/day05/input*.txt
                ${CMAKE_BINARY_DIR}/input)
