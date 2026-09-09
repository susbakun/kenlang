#pragma once

#include "expression.hpp"
#include <memory>
#include <vector>

template <typename T> class Expression;
template <typename T> class Block;
template <typename T> class Print;
template <typename T> class Var;

template <typename T> struct StmtVisitor {
  virtual T visit_block_stmt(const Block<T> &stmt) const = 0;
  virtual T visit_expression_stmt(const Expression<T> &stmt) const = 0;
  virtual T visit_print_stmt(const Print<T> &stmt) const = 0;
  virtual T visit_var_stmt(const Var<T> &stmt) const = 0;
};

template <typename T> class Stmt {
public:
  virtual T accept(const StmtVisitor<T> &) const = 0;
};

template <typename T> class Block : public Stmt<T> {
public:
  Block(std::vector<std::unique_ptr<Stmt<T>>> &statements)
      : m_statements{statements} {}

  T accept(const StmtVisitor<T> &visitor) const override;

private:
  const std::vector<std::unique_ptr<Stmt<T>>> m_statements{};
};

template <typename T> class Expression : public Stmt<T> {
public:
  Expression(const Expr<T> &expression) : m_expression{expression} {}

  T accept(const StmtVisitor<T> &visitor) const override;

private:
  const std::unique_ptr<Expr<T>> m_expression{};
};

template <typename T> class Print : public Stmt<T> {
public:
  Print(const Expr<T> &expression) : m_expression{expression} {}

  T accept(const StmtVisitor<T> &visitor) const override;

private:
  const std::unique_ptr<Expr<T>> m_expression{};
};

template <typename T> class Var : public Stmt<T> {
public:
  Var(const Token &name, const Expr<T> &initilizer)
      : m_name{name}, m_initilizer{initilizer} {}

private:
  const Token m_name;
  const Expr<T> m_initilizer{};
};
