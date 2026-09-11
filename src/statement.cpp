#include "statement.hpp"

void Expression::accept(const StmtVisitor &visitor) const {
  return visitor.visit_expression_stmt(*this);
}

void Print::accept(const StmtVisitor &visitor) const {
  return visitor.visit_print_stmt(*this);
}

void Variable::accept(const StmtVisitor &visitor) const {
  return visitor.visit_var_stmt(*this);
}
