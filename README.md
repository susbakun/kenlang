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
- The built-in `clock()` function, which returns the Unix time in seconds.
- Line comments and nested block comments.

Syntax and runtime errors are reported with their source line. The language is
still in progress: classes, inheritance, and other later chapters from the
book are not implemented yet.

## Requirements

- CMake 3.15 or newer
- A C++23-capable Clang compiler (the CMake configuration uses Homebrew LLVM)

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

## Project layout

```text
src/
  scanner.*       Tokenizes Lox source
  parser.*        Builds expression and statement ASTs
  expression.*    Expression nodes and visitor interface
  statement.*     Statement nodes and visitor interface
  interpreter.*   Evaluates expressions and executes statements
  environment.*   Chains lexical environments and variable bindings
  lox_callable.*  Interface for callable Lox values
  lox_function.*  Function calls and captured closures
  clock.*         Built-in clock callable
  runtime_error.* Represents interpreter errors with source-token context
  *_exception.hpp Implements non-local loop and return control flow
  token.*         Token representation
  lox.*           File and prompt entry points
  main.cpp        Executable entry point
test.lox          Sample program
```

## Learning resource

The implementation follows [Crafting Interpreters](https://craftinginterpreters.com/),
which builds a complete interpreter step by step. This repository may diverge
from the book where C++ design choices or additional experiments are useful.
