# Kenlang

`kenlang` is a C++ implementation of the Lox language from Robert Nystrom's
[Crafting Interpreters](https://craftinginterpreters.com/) tutorial. It is a
learning project and is being built incrementally alongside the book.

## Current progress

Kenlang can scan, parse, and interpret this subset of Lox:

- Literal values: numbers, strings, booleans, and `nil`.
- Grouping, unary operators, arithmetic, comparisons, equality, assignment,
  comma expressions, and conditional (`?:`) expressions.
- String concatenation with `+`.
- Expression statements and `print` statements.
- Variable declarations, lookup, and reassignment.
- Block statements and block-local environments.
- Line comments and nested block comments.

Syntax and runtime errors are reported with their source line. The language is
still in progress: control flow, functions, classes, and other later chapters
from the book are not implemented yet.

## Requirements

- CMake 3.15 or newer
- A C++26-capable compiler (the project currently uses Clang and `std::print`)

## Build

From the repository root:

```sh
cmake -S . -B build
cmake --build build
```

This produces the `kenlang` executable in `build/`.

## Run

Run a Lox source file:

```sh
./build/kenlang test.lox
```

Or start the prompt:

```sh
./build/kenlang
```

Submit an empty line to leave the prompt. Use `print` to display values:

```lox
var greeting = "Hello";
print greeting + " from Lox";
```

## Project layout

```text
src/
  scanner.*       Tokenizes Lox source
  parser.*        Builds expression and statement ASTs
  expression.*    Expression nodes and visitor interface
  statement.*     Statement nodes and visitor interface
  interpreter.*   Evaluates expressions and executes statements
  environment.*   Stores variable bindings for an execution scope
  runtime_error.* Represents interpreter errors with source-token context
  token.*         Token representation
  lox.*           File and prompt entry points
  main.cpp        Executable entry point
test.lox          Sample program
```

## Learning resource

The implementation follows [Crafting Interpreters](https://craftinginterpreters.com/),
which builds a complete interpreter step by step. This repository may diverge
from the book where C++ design choices or additional experiments are useful.
