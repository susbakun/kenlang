cmake_minimum_required(VERSION 3.15)
project(kenlang)

set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 26)


add_executable(${PROJECT_NAME} main.cpp token.cpp scanner.cpp lox.cpp)
