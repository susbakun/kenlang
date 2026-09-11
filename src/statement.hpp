#pragma once

#include "expression.hpp"
#include <memory>
#include <vector>

class Expression;
class Print;
class Variable;

struct StmtVisitor {
  virtual void visit_expression_stmt(const Expression &stmt) const = 0;
  virtual void visit_print_stmt(const Print &stmt) const = 0;
  virtual void visit_var_stmt(const Variable &stmt) const = 0;

  virtual ~StmtVisitor() = default;
};

class Stmt {
public:
  virtual void accept(const StmtVisitor &) const = 0;

  virtual ~Stmt() = default;
};

class Block : public Stmt {
public:
  Block(std::vector<std::unique_ptr<Stmt>> &statements)
      : m_statements{std::move(statements)} {}

  void accept(const StmtVisitor &visitor) const override;

  const std::vector<std::unique_ptr<Stmt>> m_statements{};
};

class Expression : public Stmt {
public:
  Expression(std::unique_ptr<Expr> expression)
      : m_expression{std::move(expression)} {}

  void accept(const StmtVisitor &visitor) const override;

  const std::unique_ptr<Expr> m_expression{};
};

class Print : public Stmt {
public:
  Print(std::unique_ptr<Expr> expression)
      : m_expression{std::move(expression)} {}

  void accept(const StmtVisitor &visitor) const override;

  const std::unique_ptr<Expr> m_expression{};
};

class Variable : public Stmt {
public:
  Variable(const Token &name, std::unique_ptr<Expr> initilizer)
      : m_name{name}, m_initilizer{std::move(initilizer)} {}

  void accept(const StmtVisitor &visitor) const override;

  const Token m_name;
  const std::unique_ptr<Expr> m_initilizer{};
};
