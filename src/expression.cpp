#include "expression.hpp"
#include "literal.hpp"

Object Binary::accept(const ExprVisitor &visitor) const {
  return visitor.visit_binary_expr(*this);
}

Object Grouping::accept(const ExprVisitor &visitor) const {
  return visitor.visit_grouping_expr(*this);
}

Object Literal::accept(const ExprVisitor &visitor) const {
  return visitor.visit_literal_expr(*this);
}

Object Unary::accept(const ExprVisitor &visitor) const {
  return visitor.visit_unary_expr(*this);
}

Object Ternary::accept(const ExprVisitor &visitor) const {
  return visitor.visit_ternary_expr(*this);
}
