cmake_minimum_required(VERSION 3.10)
# set the project name
project(day09)
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day09 ${SOURCES})
target_link_libraries(day09 ${CONAN_LIBS})
