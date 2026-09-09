#include "lox.hpp"
#include "literal.hpp"
#include "parser.hpp"
#include "scanner.hpp"
#include "token.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void Lox::runFile(char path[]) {
  std::ifstream fstream{path};

  if (!fstream.is_open()) {
    std::cerr << "Couldn't open the file\n";
    return;
  }

  // getting the file size
  fstream.seekg(0, std::fstream::end);
  auto file_size{fstream.tellg()};
  fstream.seekg(0, std::fstream::beg);

  std::string source{};
  source.resize(file_size);

  if (!fstream.read(reinterpret_cast<char *>(source.data()), file_size)) {
    std::cerr << "Couldn't read the file\n";
    return;
  }

  run(source);

  if (had_error)
    std::exit(65);

  if (had_runtime_error)
    std::exit(70);
}

void Lox::runPrompt() {
  while (true) {
    std::cout << "> ";
    std::string line{};
    std::getline(std::cin, line);
    if (line == "")
      break;
    run(line);
    had_error = false;
  }
}

void Lox::run(std::string &source) {
  Scanner scanner{source};
  std::vector<Token> tokens{scanner.scan_tokens()};

  Parser<Object> parser{tokens};
  auto expression{parser.parse()};

  if (had_error)
    return;

  // AstPrinter astprinter{};

  // std::cout << astprinter.print(*expression) << "\n";

  // for (const auto token : tokens) {
  //   std::cout << token << "\n";
  // }

  interpreter.interpret(*expression);
}
