#pragma once

#include "expression.hpp"
#include <cstdarg>
#include <cstddef>
#include <initializer_list>
#include <string>

class AstPrinter : public Visitor<std::string> {
public:
  inline std::string print(Expr<std::string> &expr) {
    return expr.accept(*this);
  }

  std::string visit_binary_expr(const Binary<std::string> &expr) const override;
  std::string
  visit_grouping_expr(const Grouping<std::string> &expr) const override;
  std::string
  visit_literal_expr(const Literal<std::string> &expr) const override;
  std::string visit_unary_expr(const Unary<std::string> &expr) const override;
  std::string
  visit_ternary_expr(const Ternary<std::string> &expr) const override;

private:
  std::string
  parenthesize(const std::string_view name,
               const std::initializer_list<Expr<std::string> *> exprs) const;
};
