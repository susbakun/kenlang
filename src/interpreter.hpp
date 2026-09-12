#pragma once

#include "environment.hpp"
#include "expression.hpp"
#include "literal.hpp"
#include "statement.hpp"
#include "token.hpp"
#include <memory>
#include <vector>

class Interpreter : public ExprVisitor, StmtVisitor {
public:
  Interpreter() : m_environment{std::make_shared<Environment>()} {}

  Object visit_literal_expr(Literal &expr) override;
  Object visit_grouping_expr(Grouping &expr) override;
  Object visit_unary_expr(Unary &expr) override;
  Object visit_binary_expr(Binary &expr) override;
  Object visit_ternary_expr(Ternary &expr) override;
  Object visit_variable_expr(Var &expr) override;
  Object visit_assign_expr(Assign &expr) override;
  Object visit_logical_expr(Logical &expr) override;

  void visit_expression_stmt(Expression &stmt) override;
  void visit_print_stmt(Print &stmt) override;
  void visit_var_stmt(Variable &stmt) override;
  void visit_block_stmt(Block &stmt) override;
  void visit_if_stmt(If &stmt) override;
  void visit_while_stmt(While &stmt) override;
  void visit_break_stmt(Break &stmt) override;

  void interpret(const std::vector<std::unique_ptr<Stmt>> &statements);

private:
  std::shared_ptr<Environment> m_environment;

  Object evaluate(Expr &expr);
  void execute(Stmt &stmt);
  void execute_block(const std::vector<std::unique_ptr<Stmt>> &statements,
                     std::shared_ptr<Environment> environment);

  bool is_truthy(const Object &obj) const;
  bool is_equal(const Object &a, const Object &b) const;
  void check_number_operand(const Token &op, const Object &operand) const;
  void check_number_operands(const Token &op, const Object &operand1,
                             const Object &operand2) const;
  void check_sametype_operands(const Token &op, const Object &operand1,
                               const Object &operand2) const;
  void check_zero_division(const Token &op, const Object &operand2) const;
};
