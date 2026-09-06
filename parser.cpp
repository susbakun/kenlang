#include "parser.hpp"
#include "expression.hpp"
#include "lox.hpp"
#include "tokentype.hpp"
#include <memory>
#include <sys/types.h>
#include <variant>

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::expression() {
  return comma();
}

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::comma() {
  auto expr{ternary()};

  while (match({COMMA})) {
    auto op{previous()};
    auto right{ternary()};

    expr = std::make_unique<Binary<T>>(std::move(expr), op, std::move(right));
  }

  return expr;
}

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::ternary() {
  auto expr{equality()};

  if (match({QUESTION_MARK})) {
    auto left_operator{previous()};
    auto mid{equality()};
    auto right_operator{consume(COLON, "Unexped token")};
    auto right{ternary()};

    expr = std::make_unique<Ternary<T>>(std::move(expr), left_operator,
                                        std::move(mid), right_operator,
                                        std::move(right));
  }

  return expr;
}

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::equality() {
  auto expr{comparison()};

  while (match({BANG_EQUAL, EQUAL_EQUAL})) {
    auto op{previous()};
    auto right{comparison()};

    expr = std::make_unique<Binary<T>>(std::move(expr), op, std::move(right));
  }

  return expr;
}

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::comparison() {
  auto expr{term()};

  while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    auto op{previous()};
    auto right{term()};

    expr = std::make_unique<Binary<T>>(std::move(expr), op, std::move(right));
  }

  return expr;
}

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::term() {
  auto expr{factor()};

  while (match({MINUS, PLUS})) {
    auto op{previous()};
    auto right{factor()};

    expr = std::make_unique<Binary<T>>(std::move(expr), op, std::move(right));
  }

  return expr;
}

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::factor() {
  auto expr{unary()};

  while (match({STAR, SLASH})) {
    auto op{previous()};
    auto right{unary()};

    expr = std::make_unique<Binary<T>>(std::move(expr), op, std::move(right));
  }

  return expr;
}

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::unary() {
  if (match({BANG, MINUS})) {
    auto op{previous()};
    auto right{primary()};

    return std::make_unique<Unary<T>>(op, std::move(right));
  }

  return primary();
}

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::primary() {
  if (match({FALSE}))
    return std::make_unique<Literal<T>>(false);
  if (match({TRUE}))
    return std::make_unique<Literal<T>>(true);
  if (match({NIL}))
    return std::make_unique<Literal<T>>(std::monostate{});

  if (match({NUMBER, STRING})) {
    return std::make_unique<Literal<T>>(previous().m_literal);
  }

  if (match({LEFT_PAREN})) {
    auto expr{expression()};
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_unique<Grouping<T>>(std::move(expr));
  }

  throw error(peek(), "unexpected token");
}

template <typename T>
bool Parser<T>::match(std::initializer_list<TokenType> types) {
  for (const auto &type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }

  return false;
}

template <typename T>
Token Parser<T>::consume(TokenType type, std::string_view message) {
  if (check(type))
    return advance();
  throw error(peek(), message);
}

template <typename T> bool Parser<T>::check(TokenType type) {
  if (is_at_end())
    return false;

  return (peek().m_type == type);
}

template <typename T> Token Parser<T>::advance() {
  if (!is_at_end())
    m_current++;
  return previous();
}

template <typename T> Token Parser<T>::previous() {
  return m_tokens[m_current - 1];
}

template <typename T>
ParseError Parser<T>::error(const Token &token, std::string_view message) {
  Lox::error(token, message);
  return ParseError{};
}

template <typename T> void Parser<T>::synchronize() {
  advance();

  while (!is_at_end()) {
    if (previous().m_type == SEMICOLON)
      return;

    switch (peek().m_type) {
    case CLASS:
    case FUN:
    case VAR:
    case FOR:
    case IF:
    case WHILE:
    case PRINT:
    case RETURN:
      return;
    }

    advance();
  }
}

template <typename T> Token Parser<T>::peek() { return m_tokens[m_current]; }

template <typename T> bool Parser<T>::is_at_end() {
  return peek().m_type == EOFILE;
}

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::parse() {
  try {
    return expression();
  } catch (ParseError err) {
    return nullptr;
  }
}

template class Parser<std::string>;
