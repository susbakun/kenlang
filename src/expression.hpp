#pragma once

#include "literal.hpp"
#include "token.hpp"
#include <memory>

class Binary;
class Grouping;
class Literal;
class Unary;
class Ternary;
class Var;
class Assign;
class Logical;

struct ExprVisitor {
  virtual Object visit_binary_expr(Binary &expr) = 0;
  virtual Object visit_grouping_expr(Grouping &expr) = 0;
  virtual Object visit_literal_expr(Literal &expr) = 0;
  virtual Object visit_unary_expr(Unary &expr) = 0;
  virtual Object visit_ternary_expr(Ternary &expr) = 0;
  virtual Object visit_variable_expr(Var &expr) = 0;
  virtual Object visit_assign_expr(Assign &expr) = 0;
  virtual Object visit_logical_expr(Logical &expr) = 0;

  virtual ~ExprVisitor() = default;
};

class Expr {
public:
  virtual Object accept(ExprVisitor &) = 0;

  virtual ~Expr() = default;
};

class Binary : public Expr {
public:
  Binary(std::unique_ptr<Expr> left, Token &op, std::unique_ptr<Expr> right)
      : m_left{std::move(left)}, m_op{op}, m_right{std::move(right)} {}

  Object accept(ExprVisitor &visitor) override;

  const std::unique_ptr<Expr> m_left;
  const Token m_op;
  const std::unique_ptr<Expr> m_right;
};

class Grouping : public Expr {
public:
  Grouping(std::unique_ptr<Expr> expression)
      : m_expression{std::move(expression)} {}

  Object accept(ExprVisitor &visitor) override;

  const std::unique_ptr<Expr> m_expression;
};

class Literal : public Expr {
public:
  Literal(Object value) : m_value{value} {}

  Object accept(ExprVisitor &visitor) override;

  const Object m_value{};
};

class Unary : public Expr {
public:
  Unary(Token &op, std::unique_ptr<Expr> right)
      : m_operator{op}, m_right{std::move(right)} {}

  Object accept(ExprVisitor &visitor) override;

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

  Object accept(ExprVisitor &visitor) override;

  const std::unique_ptr<Expr> m_left;
  const Token m_left_operator;
  const std::unique_ptr<Expr> m_mid;
  const Token m_right_operator;
  const std::unique_ptr<Expr> m_right;
};

class Var : public Expr {
public:
  Var(const Token &name) : m_name{name} {}

  Object accept(ExprVisitor &visitor) override;

  const Token m_name;
};

class Assign : public Expr {
public:
  Assign(const Token &name, std::unique_ptr<Expr> value)
      : m_name{name}, m_value{std::move(value)} {}

  Object accept(ExprVisitor &visitor) override;

  const Token m_name;
  const std::unique_ptr<Expr> m_value{};
};

class Logical : public Expr {
public:
  Logical(std::unique_ptr<Expr> left, Token &op, std::unique_ptr<Expr> right)
      : m_left{std::move(left)}, m_operator{op}, m_right{std::move(right)} {}

  Object accept(ExprVisitor &visitor) override;

  std::unique_ptr<Expr> m_left{};
  Token m_operator;
  std::unique_ptr<Expr> m_right{};
};
