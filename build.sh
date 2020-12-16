#!/bin/bash
export CC=clang-11 CXX=clang++-11 
mkdir -p build/bin && cd build   
conan profile show clang_profile ||
conan profile new clang_profile --detect &&
conan profile update settings.compiler=clang clang_profile  && 
conan profile update settings.compiler.version=11 clang_profile  &&
conan profile update settings.compiler.libcxx=libstdc++11 clang_profile

conan  install .. --profile clang_profile --build=missing   
cmake $1 -DENABLE_CLANG_TIDY=False -DENABLE_CPPCHECK=False ..  && 
cmake --build .  &&
cmake --build . --target copy_input_files &&
cd .. &&
for file in build/bin/* 
do    
    echo ${file}  
    time ./${file}
    printf "\n"
done
