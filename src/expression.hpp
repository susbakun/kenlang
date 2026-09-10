#pragma once

#include "literal.hpp"
#include "token.hpp"
#include <memory>

class Binary;
class Grouping;
class Literal;
class Unary;
class Ternary;

struct ExprVisitor {
  virtual Object visit_binary_expr(const Binary &expr) const = 0;
  virtual Object visit_grouping_expr(const Grouping &expr) const = 0;
  virtual Object visit_literal_expr(const Literal &expr) const = 0;
  virtual Object visit_unary_expr(const Unary &expr) const = 0;
  virtual Object visit_ternary_expr(const Ternary &expr) const = 0;

  virtual ~ExprVisitor() = default;
};

class Expr {
public:
  virtual Object accept(const ExprVisitor &) const = 0;

  virtual ~Expr() = default;
};

class Binary : public Expr {
public:
  Binary(std::unique_ptr<Expr> left, Token &op, std::unique_ptr<Expr> right)
      : m_left{std::move(left)}, m_op{op}, m_right{std::move(right)} {}

  Object accept(const ExprVisitor &visitor) const override;

  const std::unique_ptr<Expr> m_left;
  const Token m_op;
  const std::unique_ptr<Expr> m_right;
};

class Grouping : public Expr {
public:
  Grouping(std::unique_ptr<Expr> expression)
      : m_expression{std::move(expression)} {}

  Object accept(const ExprVisitor &visitor) const override;

  const std::unique_ptr<Expr> m_expression;
};

class Literal : public Expr {
public:
  Literal(Object value) : m_value{value} {}

  Object accept(const ExprVisitor &visitor) const override;

  const Object m_value{};
};

class Unary : public Expr {
public:
  Unary(Token &op, std::unique_ptr<Expr> right)
      : m_operator{op}, m_right{std::move(right)} {}

  Object accept(const ExprVisitor &visitor) const override;

  const Token m_operator;
  const std::unique_ptr<Expr> m_right;
};

class Ternary : public Expr {
public:
  Ternary(std::unique_ptr<Expr> left, Token &left_operator,
          std::unique_ptr<Expr> mid, Token &right_operator,
          std::unique_ptr<Expr> right)
      : m_left{std::move(left)}, m_left_operator{left_operator},
        m_mid{std::move(mid)}, m_right_operator{right_operator},
        m_right{std::move(right)} {}

  Object accept(const ExprVisitor &visitor) const override;

  const std::unique_ptr<Expr> m_left;
  const Token m_left_operator;
  const std::unique_ptr<Expr> m_mid;
  const Token m_right_operator;
  const std::unique_ptr<Expr> m_right;
};
