#include "../ast/expression.hpp"
#include "../token/literal.hpp"
#include "statement.hpp"
#include <memory>

Binary::Binary(std::unique_ptr<Expr> left, Token &op,
               std::unique_ptr<Expr> right)
    : m_left{std::move(left)}, m_op{op}, m_right{std::move(right)} {}

Grouping::Grouping(std::unique_ptr<Expr> expression)
    : m_expression{std::move(expression)} {}

Literal::Literal(Object value) : m_value{std::move(value)} {}

Unary::Unary(Token &op, std::unique_ptr<Expr> right)
    : m_operator{op}, m_right{std::move(right)} {}

Ternary::Ternary(std::unique_ptr<Expr> left, Token &left_operator,
                 std::unique_ptr<Expr> mid, Token &right_operator,
                 std::unique_ptr<Expr> right)
    : m_left{std::move(left)}, m_left_operator{left_operator},
      m_mid{std::move(mid)}, m_right_operator{right_operator},
      m_right{std::move(right)} {}

Var::Var(const Token &name) : m_name{name} {}

Assign::Assign(const Token &name, std::unique_ptr<Expr> value)
    : m_name{name}, m_value{std::move(value)} {}

Logical::Logical(std::unique_ptr<Expr> left, Token &op,
                 std::unique_ptr<Expr> right)
    : m_left{std::move(left)}, m_operator{op}, m_right{std::move(right)} {}

Call::Call(std::unique_ptr<Expr> callee, Token &paren,
           std::vector<std::unique_ptr<Expr>> arguments)
    : m_callee{std::move(callee)}, m_paren{paren},
      m_arguments{std::move(arguments)} {}

Anonymous::Anonymous(std::vector<Token> parameters,
                     std::vector<std::unique_ptr<Stmt>> body)
    : m_parameters{std::move(parameters)}, m_body{std::move(body)} {}

Anonymous::Anonymous(Anonymous &&) noexcept = default;

Anonymous::~Anonymous() = default;

Get::Get(std::unique_ptr<Expr> obj, Token &name)
    : m_obj{std::move(obj)}, m_name{name} {}

Set::Set(std::unique_ptr<Expr> obj, Token &name, std::unique_ptr<Expr> value)
    : m_obj{std::move(obj)}, m_name{name}, m_value{std::move(value)} {}

This::This(const Token &keyword) : m_keyword{keyword} {};

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

Object Call::accept(ExprVisitor &visitor) {
  return visitor.visit_call_expr(*this);
}

Object Anonymous::accept(ExprVisitor &visitor) {
  return visitor.visit_anonymous_func_expr(*this);
}

Object Get::accept(ExprVisitor &visitor) {
  return visitor.visit_get_expr(*this);
}

Object Set::accept(ExprVisitor &visitor) {
  return visitor.visit_set_expr(*this);
}

Object This::accept(ExprVisitor &visitor) {
  return visitor.visit_this_expr(*this);
}
