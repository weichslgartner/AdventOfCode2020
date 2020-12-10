cmake_minimum_required(VERSION 3.10)
# set the project name
project(day10)
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day10 ${SOURCES})
target_link_libraries(day10 ${CONAN_LIBS})