#include "interpreter.hpp"
#include "../ast/expression.hpp"
#include "../ast/statement.hpp"
#include "../callable/lox_callable.hpp"
#include "../callable/lox_class.hpp"
#include "../callable/lox_function.hpp"
#include "../callable/lox_instance.hpp"
#include "../errors/runtime_error.hpp"
#include "../exceptions/break_exception.hpp"
#include "../exceptions/continue_exception.hpp"
#include "../exceptions/return_exception.hpp"
#include "../lox.hpp"
#include "../token/literal.hpp"
#include "../token/tokentype.hpp"
#include "environment.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

Object Interpreter::visit_literal_expr(Literal &expr) { return expr.m_value; }

Object Interpreter::visit_grouping_expr(Grouping &expr) {
  return evaluate(*expr.m_expression);
}

Object Interpreter::visit_unary_expr(Unary &expr) {
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

Object Interpreter::visit_binary_expr(Binary &expr) {
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

Object Interpreter::visit_ternary_expr(Ternary &expr) {
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

Object Interpreter::visit_variable_expr(Var &expr) {
  return lookup_variable(expr.m_name, expr);
}

Object Interpreter::visit_assign_expr(Assign &expr) {
  auto value{evaluate(*expr.m_value)};

  auto distance{m_locals.find(&expr)};
  if (distance != m_locals.end()) {
    m_environment->assign_at(distance->second, expr.m_name, value);
  } else {
    m_globals->assign(expr.m_name, value);
  }

  return value;
}

Object Interpreter::visit_logical_expr(Logical &expr) {
  auto left{evaluate(*expr.m_left)};

  if (expr.m_operator.m_type == OR) {
    if (is_truthy(left))
      return left;
  } else if (expr.m_operator.m_type == AND) {
    if (!is_truthy(left))
      return left;
  }

  return evaluate(*expr.m_right);
}

Object Interpreter::visit_call_expr(Call &expr) {
  Object callee{evaluate(*expr.m_callee)};

  std::vector<Object> arguments{};
  for (auto &argument : expr.m_arguments) {
    arguments.push_back(std::move(evaluate(*argument)));
  }

  if (!std::holds_alternative<std::shared_ptr<LoxCallable>>(callee)) {
    throw RuntimeError{expr.m_paren, "Can only call functions and classes"};
  }

  auto function{std::get<std::shared_ptr<LoxCallable>>(callee)};

  if (arguments.size() != function->arity()) {
    throw RuntimeError{expr.m_paren,
                       "Expected " + std::to_string(function->arity()) +
                           " arguments but got " +
                           std::to_string(arguments.size()) + "."};
  }

  return function->call(*this, arguments);
}

Object Interpreter::visit_anonymous_func_expr(Anonymous &expr) {
  auto ann{std::make_shared<Anonymous>(std::move(expr))};

  return std::make_shared<LoxFunction>(ann, m_environment);
}

Object Interpreter::visit_get_expr(Get &expr) {
  auto obj{evaluate(*expr.m_obj)};

  if (std::holds_alternative<std::shared_ptr<LoxInstance>>(obj)) {
    return std::get<std::shared_ptr<LoxInstance>>(obj)->get(expr.m_name);
  }

  throw RuntimeError{expr.m_name, "Only instances can have properties."};
}

Object Interpreter::visit_set_expr(Set &expr) {
  auto obj{evaluate(*expr.m_obj)};

  if (!std::holds_alternative<std::shared_ptr<LoxInstance>>(obj)) {
    throw RuntimeError{expr.m_name, "Only instances have fields."};
  }

  auto value{evaluate(*expr.m_value)};
  std::get<std::shared_ptr<LoxInstance>>(obj)->set(expr.m_name, value);
  return value;
}

Object Interpreter::visit_this_expr(This &expr) {
  return lookup_variable(expr.m_keyword, expr);
}

void Interpreter::visit_expression_stmt(Expression &stmt) {
  evaluate(*stmt.m_expression);
}

void Interpreter::visit_print_stmt(Print &stmt) {
  auto value{evaluate(*stmt.m_expression)};
  std::cout << literal_to_string(value) << "\n";
}

void Interpreter::visit_var_stmt(Variable &stmt) {
  Object value{};
  if (stmt.m_initilizer != nullptr) {
    value = evaluate(*stmt.m_initilizer);
  }

  m_environment->define(stmt.m_name.m_lexeme, value);
}

void Interpreter::visit_block_stmt(Block &stmt) {
  execute_block(stmt.m_statements,
                std::make_shared<Environment>(m_environment));
}

void Interpreter::visit_if_stmt(If &stmt) {
  if (is_truthy(evaluate(*stmt.m_condition))) {
    execute(*stmt.m_then_branch);
  } else if (stmt.m_else_branc != nullptr) {
    execute(*stmt.m_else_branc);
  }
}

void Interpreter::visit_while_stmt(While &stmt) {
  while (is_truthy(evaluate(*stmt.m_condition))) {
    try {
      execute(*stmt.m_body);
    } catch (BreakException &exception) {
      break;
    } catch (ContinueException &exception) {
      continue;
    }
  }
}

void Interpreter::visit_break_stmt(Break &stmt) { throw BreakException{}; }

void Interpreter::visit_continue_stmt(Continue &stmt) {
  throw ContinueException{};
};

void Interpreter::visit_function_stmt(Function &stmt) {
  auto name{stmt.m_name.m_lexeme};
  auto function{std::make_shared<Function>(std::move(stmt))};

  m_environment->define(name,
                        std::make_shared<LoxFunction>(function, m_environment));
}

Object Interpreter::evaluate(Expr &expr) { return expr.accept(*this); }

bool Interpreter::is_truthy(const Object &obj) const {
  if (std::holds_alternative<std::monostate>(obj))
    return false;

  if (std::holds_alternative<bool>(obj))
    return std::get<bool>(obj);

  return true;
}

void Interpreter::visit_return_stmt(Return &stmt) {
  Object value{std::monostate{}};
  if (stmt.m_value != nullptr)
    value = evaluate(*stmt.m_value);

  throw ReturnException{value};
}

void Interpreter::visit_class_stmt(Class &stmt) {
  m_environment->define(stmt.m_name.m_lexeme, std::monostate{});

  std::map<std::string, std::shared_ptr<LoxFunction>> methods{};
  for (auto &method : stmt.m_methods) {
    auto lf{std::make_shared<LoxFunction>(method, m_environment)};

    methods.insert({method->m_name.m_lexeme, lf});
  }

  auto klass{
      std::make_shared<LoxClass>(stmt.m_name.m_lexeme, std::move(methods))};

  m_environment->assign(stmt.m_name, klass);
}

void Interpreter::resolve(Expr &expr, int depth) {
  m_locals.insert({&expr, depth});
}

Object Interpreter::lookup_variable(const Token &name, Expr &expr) {
  auto distance{m_locals.find(&expr)};

  if (distance != m_locals.end()) {
    return m_environment->get_at(distance->second, name.m_lexeme);
  } else {
    return m_globals->get(name);
  }
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
  if (operand1.index() == operand2.index())
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
    const std::vector<std::unique_ptr<Stmt>> &statements) {
  try {
    for (const auto &stmt : statements) {
      execute(*stmt);
    }
  } catch (RuntimeError &error) {
    Lox::runtime_error(error);
  }
}

void Interpreter::execute(Stmt &stmt) { stmt.accept(*this); }

void Interpreter::execute_block(
    const std::vector<std::unique_ptr<Stmt>> &statements,
    std::shared_ptr<Environment> environment) {
  auto previous{m_environment};

  try {
    m_environment = std::move(environment);
    for (const auto &statement : statements) {
      execute(*statement);
    }
  } catch (RuntimeError &error) {
    Lox::runtime_error(error);
  } catch (...) {
    m_environment = previous;
    throw;
  }

  // because cpp doesn't have fucking finally
  m_environment = previous;
}
