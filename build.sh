#!/bin/sh
mkdir -p build && cd build  && 
conan  install .. --profile clang_profile --build=missing  && 
CC=clang-11 CXX=clang++-11  cmake -DENABLE_CLANG_TIDY=True -DENABLE_CPPCHECK=True ..  && 
cmake --build .  
