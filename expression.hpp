#pragma once

#include "literal.hpp"
#include "token.hpp"
#include <memory>

template <typename T> class Binary;
template <typename T> class Grouping;
template <typename T> class Literal;
template <typename T> class Unary;

template <typename T> struct Visitor {
  virtual T visit_binary_expr(Binary<T> &expr) const = 0;
  virtual T visit_grouping_expr(Grouping<T> &expr) const = 0;
  virtual T visit_literal_expr(Literal<T> &expr) const = 0;
  virtual T visit_unary_expr(Unary<T> &expr) const = 0;
};

template <typename T> class Expr {
public:
  virtual T accept(const Visitor<T> &) = 0;

  virtual ~Expr() = default;
};

template <typename T> class Binary : public Expr<T> {
public:
  Binary(std::unique_ptr<Expr<T>> left, Token op,
         std::unique_ptr<Expr<T>> right)
      : m_left{std::move(left)}, m_op{op}, m_right{std::move(right)} {}

  T accept(const Visitor<T> &visitor) override;

  std::unique_ptr<Expr<T>> m_left;
  Token m_op;
  std::unique_ptr<Expr<T>> m_right;
};

template <typename T> class Grouping : public Expr<T> {
public:
  Grouping(std::unique_ptr<Expr<T>> expression)
      : m_expression{std::move(expression)} {}

  T accept(const Visitor<T> &visitor) override;

  std::unique_ptr<Expr<T>> m_expression;
};

template <typename T> class Literal : public Expr<T> {
public:
  Literal(Object value) : m_value{value} {}

  T accept(const Visitor<T> &visitor) override;

  Object m_value{};
};

template <typename T> class Unary : public Expr<T> {
public:
  Unary(Token op, std::unique_ptr<Expr<T>> right)
      : m_operator{op}, m_right{std::move(right)} {}

  T accept(const Visitor<T> &visitor) override;

  Token m_operator;
  std::unique_ptr<Expr<T>> m_right;
};
