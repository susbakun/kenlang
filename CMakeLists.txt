project(kenlang)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_COMPILER clang++)


add_executable(${PROJECT_NAME} main.cpp)