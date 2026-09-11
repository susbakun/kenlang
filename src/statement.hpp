#pragma once

#include "expression.hpp"
#include <memory>
#include <vector>

class Expression;
class Block;
class Print;
class Variable;
class If;

struct StmtVisitor {
  virtual void visit_expression_stmt(Expression &stmt) = 0;
  virtual void visit_print_stmt(Print &stmt) = 0;
  virtual void visit_var_stmt(Variable &stmt) = 0;
  virtual void visit_block_stmt(Block &stmt) = 0;
  virtual void visit_if_stmt(If &stmt) = 0;

  virtual ~StmtVisitor() = default;
};

class Stmt {
public:
  virtual void accept(StmtVisitor &) = 0;

  virtual ~Stmt() = default;
};

class Block : public Stmt {
public:
  Block(std::vector<std::unique_ptr<Stmt>> &&statements)
      : m_statements{std::move(statements)} {}

  void accept(StmtVisitor &visitor) override;

  const std::vector<std::unique_ptr<Stmt>> m_statements{};
};

class Expression : public Stmt {
public:
  Expression(std::unique_ptr<Expr> expression)
      : m_expression{std::move(expression)} {}

  void accept(StmtVisitor &visitor) override;

  const std::unique_ptr<Expr> m_expression{};
};

class Print : public Stmt {
public:
  Print(std::unique_ptr<Expr> expression)
      : m_expression{std::move(expression)} {}

  void accept(StmtVisitor &visitor) override;

  const std::unique_ptr<Expr> m_expression{};
};

class Variable : public Stmt {
public:
  Variable(const Token &name, std::unique_ptr<Expr> initilizer)
      : m_name{name}, m_initilizer{std::move(initilizer)} {}

  void accept(StmtVisitor &visitor) override;

  const Token m_name;
  const std::unique_ptr<Expr> m_initilizer{};
};

class If : public Stmt {
public:
  If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch,
     std::unique_ptr<Stmt> else_branch)
      : m_condition{std::move(condition)},
        m_then_branch{std::move(then_branch)},
        m_else_branc{std::move(else_branch)} {}

  void accept(StmtVisitor &visitor) override;

  std::unique_ptr<Expr> m_condition{};
  std::unique_ptr<Stmt> m_then_branch{};
  std::unique_ptr<Stmt> m_else_branc{};
};
