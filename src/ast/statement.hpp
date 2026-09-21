#pragma once

#include "../token/token.hpp"
#include "expression.hpp"
#include <memory>
#include <vector>

class Expression;
class Block;
class Print;
class Variable;
class If;
class While;
class Break;
class Continue;
class Function;
class Return;
class Class;

struct StmtVisitor {
  virtual void visit_expression_stmt(Expression &stmt) = 0;
  virtual void visit_print_stmt(Print &stmt) = 0;
  virtual void visit_var_stmt(Variable &stmt) = 0;
  virtual void visit_block_stmt(Block &stmt) = 0;
  virtual void visit_if_stmt(If &stmt) = 0;
  virtual void visit_while_stmt(While &stmt) = 0;
  virtual void visit_break_stmt(Break &stmt) = 0;
  virtual void visit_continue_stmt(Continue &stmt) = 0;
  virtual void visit_function_stmt(Function &stmt) = 0;
  virtual void visit_return_stmt(Return &stmt) = 0;
  virtual void visit_class_stmt(Class &stmt) = 0;

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

class While : public Stmt {
public:
  While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
      : m_condition{std::move(condition)}, m_body{std::move(body)} {}

  void accept(StmtVisitor &visitor) override;

  std::unique_ptr<Expr> m_condition{};
  std::unique_ptr<Stmt> m_body{};
};

class Break : public Stmt {
public:
  Break(Token &keyword) : m_keyword{keyword} {}

  void accept(StmtVisitor &visitor) override;

  Token m_keyword;
};

class Continue : public Stmt {
public:
  Continue(Token &keyword) : m_keyword{keyword} {}

  void accept(StmtVisitor &visitor) override;

  Token m_keyword;
};

class Function : public Stmt {
public:
  Function(Token &name, std::vector<Token> parameters,
           std::vector<std::unique_ptr<Stmt>> body)
      : m_name{name}, m_parameters{std::move(parameters)},
        m_body{std::move(body)} {}

  void accept(StmtVisitor &visitor) override;

  Token m_name;
  std::vector<Token> m_parameters{};
  std::vector<std::unique_ptr<Stmt>> m_body{};
};

class Return : public Stmt {
public:
  Return(Token &keyword, std::unique_ptr<Expr> value)
      : m_keyword{keyword}, m_value{std::move(value)} {}

  void accept(StmtVisitor &visitor) override;

  Token m_keyword;
  std::unique_ptr<Expr> m_value{};
};

class Class : public Stmt {
public:
  Class(Token &name, std::unique_ptr<Var> superclass,
        std::vector<std::shared_ptr<Function>> methods)
      : m_name{name}, m_superclass{std::move(superclass)},
        m_methods{std::move(methods)} {}

  void accept(StmtVisitor &visitor);

  Token m_name;
  std::unique_ptr<Var> m_superclass;
  std::vector<std::shared_ptr<Function>> m_methods;
};
