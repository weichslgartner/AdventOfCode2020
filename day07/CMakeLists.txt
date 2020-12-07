cmake_minimum_required(VERSION 3.10)
project(day07)
file(GLOB SOURCES *.cpp)
# add the executable
add_executable(day07 ${SOURCES})
target_link_libraries(day07 ${CONAN_LIBS})