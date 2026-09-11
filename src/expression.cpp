#include "expression.hpp"
#include "literal.hpp"

Object Binary::accept(ExprVisitor &visitor) {
  return visitor.visit_binary_expr(*this);
}

Object Grouping::accept(ExprVisitor &visitor) {
  return visitor.visit_grouping_expr(*this);
}

Object Literal::accept(ExprVisitor &visitor) {
  return visitor.visit_literal_expr(*this);
}

Object Unary::accept(ExprVisitor &visitor) {
  return visitor.visit_unary_expr(*this);
}

Object Ternary::accept(ExprVisitor &visitor) {
  return visitor.visit_ternary_expr(*this);
}

Object Var::accept(ExprVisitor &visitor) {
  return visitor.visit_variable_expr(*this);
}

Object Assign::accept(ExprVisitor &visitor) {
  return visitor.visit_assign_expr(*this);
}

Object Logical::accept(ExprVisitor &visitor) {
  return visitor.visit_logical_expr(*this);
}
