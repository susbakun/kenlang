#include <cstddef>
#include <iostream>

#include "lox.hpp"

int main(int argc, char *argv[]) {
  Lox lox{};

  if (argc > 2) {
    std::cerr << "too many argmuents" << "\n";
    return 64;
  } else if (argc == 2) {
    lox.runFile(argv[1]);
  } else {
    lox.runPrompt();
  }

  return 0;
}
