#pragma once

#include <string>

#include "literal.hpp"
#include "tokentype.hpp"

class Token {
public:
  Token(TokenType type, std::string &lexeme, Literal &literal, int line)
      : m_type{type}, m_lexeme{lexeme}, m_literal{literal}, m_line{line} {}

  friend std::ostream &operator<<(std::ostream &out, const Token &token) {
    out << static_cast<std::string>(token);
    return out;
  };

  operator std::string() const;

private:
  TokenType m_type;
  std::string m_lexeme;
  Literal m_literal;
  int m_line;
};
