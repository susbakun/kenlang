#pragma once

#include "expression.hpp"
#include "statement.hpp"
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

class Parser {
public:
  Parser(const std::vector<Token> &tokens) : m_tokens{tokens} {}
  std::vector<std::unique_ptr<Stmt>> parse();

private:
  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> comma();
  std::unique_ptr<Expr> ternary();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> primary();

  std::unique_ptr<Stmt> declration();
  std::unique_ptr<Stmt> var_declration();
  std::unique_ptr<Stmt> statement();
  std::unique_ptr<Stmt> print_statement();
  std::unique_ptr<Stmt> expression_statement();

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
