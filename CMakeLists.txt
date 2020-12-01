cmake_minimum_required(VERSION 3.19)

# set the project name
project(adventofcode20)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
enable_testing()
find_package(fmt)
find_package(GTest)
add_subdirectory(day01)
