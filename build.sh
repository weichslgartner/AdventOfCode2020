#!/bin/bash
mkdir -p build/bin && cd build   
if [ ! 'profile show clang_profile' ] ; then
       conan profile new clang_profile --detect &&
       conan profile update settings.compiler=clang clang_profile  && 
       conan profile update settings.compiler.version=10 clang_profile  &&
       conan profile update settings.compiler.libcxx=libstdc++11 clang_profile
fi
conan  install .. --profile clang_profile --build=missing   
CC=clang-11 CXX=clang++-11  cmake $1 -DENABLE_CLANG_TIDY=False -DENABLE_CPPCHECK=False ..  && 
cmake --build .  &&
cmake --build . --target copy_input_files &&
cd .. &&
for file in build/bin/* 
do    
    echo ${file}  
    time ./${file}
    printf "\n"
done
