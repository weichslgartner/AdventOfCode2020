cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set the project name
project(day01)

# add the executable
add_executable(day01 day01.cpp)
target_link_libraries(day01 ${CONAN_LIBS})
message(STATUS "conan_libs_path ${CONAN_LIBS}")