#include "statement.hpp"

void Expression::accept(StmtVisitor &visitor) {
  return visitor.visit_expression_stmt(*this);
}

void Print::accept(StmtVisitor &visitor) {
  return visitor.visit_print_stmt(*this);
}

void Variable::accept(StmtVisitor &visitor) {
  return visitor.visit_var_stmt(*this);
}
