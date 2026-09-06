# Kenlang

`kenlang` is a C++ implementation of the Lox language from Robert Nystrom's
[Crafting Interpreters](https://craftinginterpreters.com/) tutorial. It is a
learning project and is being built incrementally alongside the book.

## Current progress

The project currently implements the front end for Lox expressions:

- Scans Lox source into tokens, including literals, keywords, line comments,
  and nested block comments.
- Parses literals, grouping, unary operators, arithmetic and comparison
  operators, equality, comma expressions, and conditional (`?:`) expressions.
- Represents expressions as an AST using the visitor pattern.
- Prints the parsed AST and the scanned token stream.

It does not yet evaluate expressions or implement Lox statements, variables,
functions, classes, or a runtime.

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

Submit an empty line to leave the prompt. At this stage, each successful input
prints its AST followed by its tokens. For example, `1 + 2 * 3` is printed as
an AST that preserves multiplication's higher precedence.

## Project layout

```text
src/
  scanner.*       Tokenizes Lox source
  parser.*        Builds expression ASTs
  expression.*    AST node types and visitor interface
  astprinter.*    Renders an AST for inspection
  token.*         Token representation
  lox.*           File and prompt entry points
  main.cpp        Executable entry point
test.lox          Small sample expression
```

## Learning resource

The implementation follows [Crafting Interpreters](https://craftinginterpreters.com/),
which builds a complete interpreter step by step. This repository may diverge
from the book where C++ design choices or additional experiments are useful.
