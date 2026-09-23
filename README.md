# Kenlang

`kenlang` is a C++ implementation of the Lox language from Robert Nystrom's
[Crafting Interpreters](https://craftinginterpreters.com/) tutorial. It is a
learning project and is being built incrementally alongside the book.

## Current progress

Kenlang can scan, parse, and interpret this subset of Lox:

- Literal values: numbers, strings, booleans, and `nil`.
- Grouping, unary operators, arithmetic, comparisons, equality, assignment,
  comma expressions, and conditional (`?:`) expressions.
- String concatenation with `+`, plus short-circuiting `and` and `or`.
- Expression statements, `print`, and lexically scoped variables.
- Blocks, `if`/`else`, `while`, `for`, and `break`.
- Named functions, anonymous functions, calls, parameters, `return`, and
  closures.
- Classes, instances, fields, methods, initializers, getters, inheritance,
  `this`, and `super`.
- The built-in `clock()` function, which returns the Unix time in seconds.
- Line comments and nested block comments.

Syntax and runtime errors are reported with their source line. The language is
resolved before execution, allowing it to report invalid local-variable use,
invalid `this`/`super` usage, and unused local variables. The language is
still in progress and may diverge from the book for C++ experiments.

## Requirements

- CMake 3.15 or newer
- A C++23-capable compiler

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

Functions are values and may be passed as arguments:

```lox
fun twice(fn) {
  fn(1);
  fn(2);
}

twice(fun (value) {
  print value;
});
```

Classes support methods and inheritance:

```lox
class Doughnut {
  cook() { print "Fry until golden brown."; }
}

class BostonCream < Doughnut {
  cook() {
    super.cook();
    print "Boston Cream";
  }
}

BostonCream().cook();
```

## Project layout

```text
src/
  scan/           Tokenizes Lox source
  token/          Token types, tokens, and runtime values
  ast/            Expression/statement ASTs and the parser
  execute/        Interpreter and chained lexical environments
  callable/       Functions, closures, classes, instances, and `clock()`
  resolve/        Static local-variable resolution and semantic checks
  errors/         Runtime errors with source-token context
  exceptions/     Non-local loop and return control flow
  lox.*           File and prompt entry points
  main.cpp        Executable entry point
test.lox          Sample inheritance program
```

## Learning resource

The implementation follows [Crafting Interpreters](https://craftinginterpreters.com/),
which builds a complete interpreter step by step. This repository may diverge
from the book where C++ design choices or additional experiments are useful.
