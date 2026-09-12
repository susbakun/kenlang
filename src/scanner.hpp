#pragma once

#include "literal.hpp"
#include "token.hpp"
#include "tokentype.hpp"
#include <map>
#include <string>
#include <vector>

class Scanner {
public:
  Scanner(std::string &source) : m_source{source} {}
  std::vector<Token> scan_tokens();

private:
  void scan_token();
  void identifier();
  void number();
  void string();
  bool match(char expected);
  char peek();
  char peek_next();
  char advance();
  void add_token(TokenType type);
  void add_token(TokenType type, Object literal);
  bool is_at_end() const;

  std::string m_source{};
  std::vector<Token> m_tokens{};

  int m_start{0};
  int m_current{0};
  int m_line{1};

  inline static const std::map<std::string, TokenType> m_keywords{
      {"and", AND},    {"class", CLASS}, {"else", ELSE},     {"false", FALSE},
      {"for", FOR},    {"fun", FUN},     {"if", IF},         {"nil", NIL},
      {"or", OR},      {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
      {"this", THIS},  {"true", TRUE},   {"var", VAR},       {"while", WHILE},
      {"break", BREAK}};
};
