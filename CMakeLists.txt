cmake_minimum_required(VERSION 3.15)


project(kenlang)


set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} src/main.cpp src/token.cpp
  src/scanner.cpp src/lox.cpp
  src/expression.cpp src/parser.cpp
  src/interpreter.cpp
  src/statement.cpp
  src/environment.cpp
  src/clock.cpp
  src/lox_function.cpp
  )
