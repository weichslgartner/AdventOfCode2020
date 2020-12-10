#!/bin/bash
mkdir -p build/bin && cd build   
conan profile show gcc_profile ||
conan profile new gcc_profile --detect &&
conan profile update settings.compiler=gcc gcc_profile  && 
conan profile update settings.compiler.version=10 gcc_profile  &&
conan profile update settings.compiler.libcxx=libstdc++11 gcc_profile

conan  install .. --profile gcc_profile --build=missing   
CC=gcc-10 CXX=g++-10 cmake $1 -DENABLE_CLANG_TIDY=False -DENABLE_CPPCHECK=False ..  && 
cmake --build .  &&
cmake --build . --target copy_input_files &&
cd .. &&
for file in build/bin/* 
do    
    echo ${file}  
    time ./${file}
    printf "\n"
done
