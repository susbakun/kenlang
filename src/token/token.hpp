#pragma once

#include <string>

#include "literal.hpp"
#include "tokentype.hpp"

class Token {
public:
  Token(TokenType type, const std::string &lexeme, const Object &literal,
        int line)
      : m_type{type}, m_lexeme{lexeme}, m_literal{literal}, m_line{line} {}

  friend std::ostream &operator<<(std::ostream &out, const Token &token) {
    out << static_cast<std::string>(token);
    return out;
  };

  operator std::string() const;

  TokenType m_type;
  std::string m_lexeme;
  Object m_literal;
  int m_line;
};
