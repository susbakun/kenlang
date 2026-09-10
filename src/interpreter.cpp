#include "interpreter.hpp"
#include "expression.hpp"
#include "literal.hpp"
#include "lox.hpp"
#include "runtime_error.hpp"
#include "statement.hpp"
#include "tokentype.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

Object Interpreter::visit_literal_expr(const Literal &expr) const {
  return expr.m_value;
}

Object Interpreter::visit_grouping_expr(const Grouping &expr) const {
  return evaluate(*expr.m_expression);
}

Object Interpreter::visit_unary_expr(const Unary &expr) const {
  auto right{evaluate(*expr.m_right)};

  switch (expr.m_operator.m_type) {
  case MINUS:
    return -std::get<double>(right);

  case BANG:
    return !is_truthy(right);

  default:
    return std::monostate{};
  }
}

Object Interpreter::visit_binary_expr(const Binary &expr) const {
  auto left{evaluate(*expr.m_left)};
  auto right{evaluate(*expr.m_right)};

  switch (expr.m_op.m_type) {
  case GREATER:
    check_number_operands(expr.m_op, left, right);
    return std::get<double>(left) > std::get<double>(right);

  case GREATER_EQUAL:
    check_number_operands(expr.m_op, left, right);
    return std::get<double>(left) >= std::get<double>(right);

  case LESS:
    check_number_operands(expr.m_op, left, right);
    return std::get<double>(left) < std::get<double>(right);

  case LESS_EQUAL:
    check_number_operands(expr.m_op, left, right);
    return std::get<double>(left) <= std::get<double>(right);

  case MINUS:
    check_number_operands(expr.m_op, left, right);
    return std::get<double>(left) - std::get<double>(right);

  case PLUS:
    if (std::holds_alternative<double>(left) &&
        std::holds_alternative<double>(right)) {
      return std::get<double>(left) + std::get<double>(right);
    }

    if (std::holds_alternative<std::string>(left) &&
        std::holds_alternative<std::string>(right)) {
      return std::get<std::string>(left) + std::get<std::string>(right);
    }

    if (std::holds_alternative<std::string>(left) ||
        std::holds_alternative<std::string>(right)) {
      return literal_to_string(left) + literal_to_string(right);
    }

    throw RuntimeError(expr.m_op,
                       "Operands must be two numbers or two strings.");

  case STAR:
    check_number_operands(expr.m_op, left, right);
    return std::get<double>(left) * std::get<double>(right);

  case SLASH:
    check_number_operands(expr.m_op, left, right);
    check_zero_division(expr.m_op, right);
    return std::get<double>(left) / std::get<double>(right);

  case EQUAL_EQUAL:
    return is_equal(left, right);
  case BANG_EQUAL:
    return !is_equal(left, right);

  case COLON:
    return right;

  default:
    return std::monostate{};
  }
}

Object Interpreter::visit_ternary_expr(const Ternary &expr) const {
  auto left{evaluate(*expr.m_left)};
  auto mid{evaluate(*expr.m_mid)};
  auto right{evaluate(*expr.m_right)};

  if (!std::holds_alternative<bool>(left)) {
    throw RuntimeError(expr.m_left_operator, "Left operand must be a boolean.");
  }

  check_sametype_operands(expr.m_left_operator, left, right);

  auto left_bool{std::get<bool>(left)};
  if (left_bool) {
    return mid;
  } else {
    return right;
  }
}

void Interpreter::visit_expression_stmt(const Expression &stmt) const {
  evaluate(*stmt.m_expression);
}

void Interpreter::visit_print_stmt(const Print &stmt) const {
  auto value{evaluate(*stmt.m_expression)};
  std::cout << literal_to_string(value) << "\n";
}

Object Interpreter::evaluate(const Expr &expr) const {
  return expr.accept(*this);
}

bool Interpreter::is_truthy(const Object &obj) const {
  if (std::holds_alternative<std::monostate>(obj))
    return false;

  if (std::holds_alternative<bool>(obj))
    return std::get<bool>(obj);

  return true;
}

bool Interpreter::is_equal(const Object &a, const Object &b) const {
  if (std::holds_alternative<std::monostate>(a) &&
      std::holds_alternative<std::monostate>(b)) {
    return true;
  }

  if (std::holds_alternative<std::monostate>(a))
    return false;

  return a == b;
}

void Interpreter::check_number_operand(const Token &op,
                                       const Object &operand) const {
  if (std::holds_alternative<double>(operand))
    return;
  throw RuntimeError{op, "Operand must be a number"};
}

void Interpreter::check_number_operands(const Token &op, const Object &operand1,
                                        const Object &operand2) const {
  if (std::holds_alternative<double>(operand1) &&
      std::holds_alternative<double>(operand2))
    return;
  throw RuntimeError{op, "Operands must be numbers"};
}

void Interpreter::check_sametype_operands(const Token &op,
                                          const Object &operand1,
                                          const Object &operand2) const {
  if (std::is_same_v<decltype(operand1), decltype(operand2)>)
    return;

  throw RuntimeError{op, "Operands must be of the same type"};
}

void Interpreter::check_zero_division(const Token &op,
                                      const Object &operand2) const {
  if (std::get<double>(operand2) != 0.0)
    return;

  throw RuntimeError{op, "Zero division error"};
}

void Interpreter::interpret(
    const std::vector<std::unique_ptr<Stmt>> &statements) const {
  try {
    for (const auto &stmt : statements) {
      execute(*stmt);
    }
  } catch (RuntimeError &error) {
    Lox::runtime_error(error);
  }
}

void Interpreter::execute(const Stmt &stmt) const { stmt.accept(*this); }
