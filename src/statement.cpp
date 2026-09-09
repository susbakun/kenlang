#include "statement.hpp"

template <typename T> T Block<T>::accept(const StmtVisitor<T> &visitor) const {
  return visitor.visit_block_stmt(*this);
}

template <typename T>
T Expression<T>::accept(const StmtVisitor<T> &visitor) const {
  return visitor.visit_expression_stmt(*this);
}

template <typename T> T Print<T>::accept(const StmtVisitor<T> &visitor) const {
  return visitor.visit_print_stmt(*this);
}
