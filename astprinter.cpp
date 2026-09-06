#include "astprinter.hpp"
#include <variant>

std::string AstPrinter::visit_binary_expr(Binary<std::string> &expr) const {
  return parenthesize(expr.m_op.m_lexeme,
                      {expr.m_left.get(), expr.m_right.get()});
}

std::string AstPrinter::visit_grouping_expr(Grouping<std::string> &expr) const {
  return parenthesize("group", {expr.m_expression.get()});
}

std::string AstPrinter::visit_literal_expr(Literal<std::string> &expr) const {
  if (std::holds_alternative<std::monostate>(expr.m_value))
    return "nil";

  return literal_to_string(expr.m_value);
}

std::string AstPrinter::visit_unary_expr(Unary<std::string> &expr) const {
  return parenthesize(expr.m_operator.m_lexeme, {expr.m_right.get()});
}

std::string AstPrinter::parenthesize(
    std::string_view name,
    std::initializer_list<Expr<std::string> *> exprs) const {
  std::string output{};

  output += "(";
  output += name;

  for (Expr<std::string> *expr : exprs) {
    output += " ";
    output += expr->accept(*this);
  }

  output += ")";

  return output;
}
