#pragma once

#include "expression.hpp"
#include "literal.hpp"
#include "token.hpp"

class Interpreter : public ExprVisitor<Object> {
public:
  Object visit_literal_expr(const Literal<Object> &expr) const override;
  Object visit_grouping_expr(const Grouping<Object> &expr) const override;
  Object visit_unary_expr(const Unary<Object> &expr) const override;
  Object visit_binary_expr(const Binary<Object> &expr) const override;
  Object visit_ternary_expr(const Ternary<Object> &expr) const override;

  void interpret(const Expr<Object> &expr) const;

private:
  Object evaluate(const Expr<Object> &expr) const;
  bool is_truthy(const Object &obj) const;
  bool is_equal(const Object &a, const Object &b) const;
  void check_number_operand(const Token &op, const Object &operand) const;
  void check_number_operands(const Token &op, const Object &operand1,
                             const Object &operand2) const;
  void check_sametype_operands(const Token &op, const Object &operand1,
                               const Object &operand2) const;
  void check_zero_division(const Token &op, const Object &operand2) const;
};
