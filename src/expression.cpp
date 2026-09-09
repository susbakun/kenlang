#include "expression.hpp"
#include "literal.hpp"

template <typename T> T Binary<T>::accept(const ExprVisitor<T> &visitor) const {
  return visitor.visit_binary_expr(*this);
}

template <typename T>
T Grouping<T>::accept(const ExprVisitor<T> &visitor) const {
  return visitor.visit_grouping_expr(*this);
}

template <typename T>
T Literal<T>::accept(const ExprVisitor<T> &visitor) const {
  return visitor.visit_literal_expr(*this);
}

template <typename T> T Unary<T>::accept(const ExprVisitor<T> &visitor) const {
  return visitor.visit_unary_expr(*this);
}

template <typename T>
T Ternary<T>::accept(const ExprVisitor<T> &visitor) const {
  return visitor.visit_ternary_expr(*this);
}

template class Binary<std::string>;
template class Grouping<std::string>;
template class Literal<std::string>;
template class Unary<std::string>;
template class Ternary<std::string>;
template class Binary<Object>;
template class Grouping<Object>;
template class Literal<Object>;
template class Unary<Object>;
template class Ternary<Object>;
