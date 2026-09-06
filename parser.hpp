#pragma once

#include "expression.hpp"
#include "token.hpp"
#include "tokentype.hpp"
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

class ParseError : public std::runtime_error {
public:
  ParseError() : std::runtime_error{message} {}
  std::string message{};
};

template <typename T> class Parser {
public:
  Parser(const std::vector<Token> &tokens) : m_tokens{tokens} {}
  std::unique_ptr<Expr<T>> parse();

private:
  std::unique_ptr<Expr<T>> expression();
  std::unique_ptr<Expr<T>> equality();
  std::unique_ptr<Expr<T>> comparison();
  std::unique_ptr<Expr<T>> term();
  std::unique_ptr<Expr<T>> factor();
  std::unique_ptr<Expr<T>> unary();
  std::unique_ptr<Expr<T>> primary();

  bool match(std::initializer_list<TokenType> types);
  Token consume(TokenType type, std::string_view message);
  bool check(TokenType type);
  Token advance();
  Token previous();
  ParseError error(const Token &token, std::string_view message);
  void synchronize();
  Token peek();
  bool is_at_end();

  std::vector<Token> m_tokens{};
  int m_current{};
};
