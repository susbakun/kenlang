#pragma once

#include "expression.hpp"
#include "literal.hpp"
#include "statement.hpp"
#include "token.hpp"

class Interpreter : public ExprVisitor, StmtVisitor {
public:
  Object visit_literal_expr(const Literal &expr) const override;
  Object visit_grouping_expr(const Grouping &expr) const override;
  Object visit_unary_expr(const Unary &expr) const override;
  Object visit_binary_expr(const Binary &expr) const override;
  Object visit_ternary_expr(const Ternary &expr) const override;

  void visit_expression_stmt(const Expression &stmt) const override;
  void visit_print_stmt(const Print &stmt) const override;

  void interpret(const std::vector<std::unique_ptr<Stmt>> &statements) const;

private:
  Object evaluate(const Expr &expr) const;
  void execute(const Stmt &stmt) const;

  bool is_truthy(const Object &obj) const;
  bool is_equal(const Object &a, const Object &b) const;
  void check_number_operand(const Token &op, const Object &operand) const;
  void check_number_operands(const Token &op, const Object &operand1,
                             const Object &operand2) const;
  void check_sametype_operands(const Token &op, const Object &operand1,
                               const Object &operand2) const;
  void check_zero_division(const Token &op, const Object &operand2) const;
};
