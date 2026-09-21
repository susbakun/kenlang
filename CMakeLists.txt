cmake_minimum_required(VERSION 3.15)
project(kenlang)


set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
    src/main.cpp

    src/token/token.cpp
    src/token/literal.cpp

    src/scan/scanner.cpp

    src/ast/parser.cpp
    src/ast/expression.cpp
    src/ast/statement.cpp


    src/execute/interpreter.cpp
    src/execute/environment.cpp

    src/callable/clock.cpp
    src/callable/lox_function.cpp
    src/callable/lox_class.cpp
    src/callable/lox_instance.cpp

    src/resolve/resolver.cpp

    src/lox.cpp
)
