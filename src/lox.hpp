#pragma once

#include "token.hpp"
#include "tokentype.hpp"
#include <print>
#include <string_view>

class Lox {
public:
  Lox() = default;

  void runFile(char path[]);
  void runPrompt();
  void run(std::string &source);

  static inline void error(int line, std::string_view error) {
    report(line, "", error);
  };

  static inline void error(const Token &token, std::string_view message) {
    if (token.m_type == EOFILE) {
      report(token.m_line, " at end", message);
    } else {
      report(token.m_line, " at '" + token.m_lexeme + "'", message);
    }
  }

  static inline void report(int line, std::string_view where,
                            std::string_view error) {
    std::println("[line {}] Error{}: {}", line, where, error);
    had_error = true;
  }

private:
  static inline bool had_error{false};
};
