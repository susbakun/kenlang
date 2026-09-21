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
  std::unique_ptr<Expr> assignment();
  std::unique_ptr<Expr> ternary();
  std::unique_ptr<Expr> orexp();
  std::unique_ptr<Expr> andexp();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> call();
  std::unique_ptr<Expr> finish_call(std::unique_ptr<Expr> callee);
  std::unique_ptr<Expr> primary();
  std::unique_ptr<Expr> anonymous_function();

  std::unique_ptr<Stmt> declaration();
  std::unique_ptr<Class> class_declration();
  std::unique_ptr<Function> function(std::string_view kind);
  std::unique_ptr<Variable> var_declaration();
  std::unique_ptr<Stmt> statement();
  std::unique_ptr<Stmt> for_statement();
  std::unique_ptr<If> if_statement();
  std::unique_ptr<Print> print_statement();
  std::unique_ptr<Return> return_statement();
  std::unique_ptr<While> while_statement();
  std::unique_ptr<Break> break_statement();
  std::vector<std::unique_ptr<Stmt>> block();
  std::unique_ptr<Expression> expression_statement();

  bool match(std::initializer_list<TokenType> types);
  Token consume(TokenType type, std::string_view message);
  bool check(TokenType type);
  bool check_next(TokenType type);
  Token advance();
  Token previous();
  ParseError error(const Token &token, std::string_view message);
  void synchronize();
  Token peek();
  bool is_at_end();

  std::vector<Token> m_tokens{};
  int m_current{};
  int m_loop_depth{};
};
