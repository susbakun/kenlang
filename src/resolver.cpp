#include "resolver.hpp"
#include "expression.hpp"
#include "literal.hpp"
#include "lox.hpp"
#include <string>
#include <unordered_map>
#include <variant>

Object Resolver::visit_variable_expr(Var &expr) {
  if (!m_scopes.empty()) {
    auto &scope{m_scopes.top()};
    auto it{scope.find(expr.m_name.m_lexeme)};
    if (it != scope.end() && it->second == false) {
      Lox::error(expr.m_name,
                 "Can't read local variable in its own initializer.");
    }
  }

  resolve_local(expr, expr.m_name);
  return std::monostate{};
}

Object Resolver::visit_assign_expr(Assign &expr) {
  resolve(*expr.m_value);
  resolve_local(expr, expr.m_name);

  return std::monostate{};
}

Object Resolver::visit_binary_expr(Binary &expr) {
  resolve(*expr.m_left);
  resolve(*expr.m_right);

  return std::monostate{};
}

Object Resolver::visit_ternary_expr(Ternary &expr) {
  resolve(*expr.m_left);
  resolve(*expr.m_mid);
  resolve(*expr.m_right);

  return std::monostate{};
}

Object Resolver::visit_call_expr(Call &expr) {
  resolve(*expr.m_callee);

  for (auto &argument : expr.m_arguments) {
    resolve(*argument);
  }

  return std::monostate{};
}

Object Resolver::visit_grouping_expr(Grouping &expr) {
  resolve(*expr.m_expression);
  return std::monostate{};
}

Object Resolver::visit_literal_expr(Literal &expr) { return std::monostate{}; }

Object Resolver::visit_logical_expr(Logical &expr) {
  resolve(*expr.m_left);
  resolve(*expr.m_right);

  return std::monostate{};
}

Object Resolver::visit_unary_expr(Unary &expr) {
  resolve(*expr.m_right);

  return std::monostate{};
}

Object Resolver::visit_anonymous_func_expr(Anonymous &expr) {
  resolve_function(expr);

  return std::monostate{};
}

void Resolver::visit_block_stmt(Block &stmt) {
  begin_scope();
  resolve(stmt.m_statements);
  end_scope();
}

void Resolver::visit_var_stmt(Variable &stmt) {
  declare(stmt.m_name);
  if (stmt.m_initilizer != nullptr) {
    resolve(*stmt.m_initilizer);
  }
  define(stmt.m_name);
}

void Resolver::visit_function_stmt(Function &stmt) {
  declare(stmt.m_name);
  define(stmt.m_name);

  resolve_function(stmt);
}

void Resolver::visit_break_stmt(Break &stmt) { return; }
void Resolver::visit_continue_stmt(Continue &stmt) { return; }

void Resolver::visit_expression_stmt(Expression &stmt) {
  resolve(*stmt.m_expression);
}

void Resolver::visit_if_stmt(If &stmt) {
  resolve(*stmt.m_condition);
  resolve(*stmt.m_then_branch);

  if (stmt.m_else_branc != nullptr)
    resolve(*stmt.m_else_branc);
}

void Resolver::visit_print_stmt(Print &stmt) { resolve(*stmt.m_expression); }

void Resolver::visit_return_stmt(Return &stmt) {
  if (stmt.m_value != nullptr)
    resolve(*stmt.m_value);
}

void Resolver::visit_while_stmt(While &stmt) {
  resolve(*stmt.m_condition);
  resolve(*stmt.m_body);
}

void Resolver::begin_scope() {
  m_scopes.push(std::unordered_map<std::string, bool>{});
}

void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>> &statements) {
  for (const auto &stmt : statements) {
    stmt->accept(*this);
  }
}

void Resolver::resolve(Stmt &stmt) { stmt.accept(*this); }

void Resolver::resolve(Expr &expr) { expr.accept(*this); }

void Resolver::end_scope() { m_scopes.pop(); }

void Resolver::declare(const Token &token) {
  if (m_scopes.empty())
    return;

  auto &scope{m_scopes.top()};

  scope.insert({token.m_lexeme, false});
}

void Resolver::define(const Token &token) {
  if (m_scopes.empty())
    return;

  auto &scope{m_scopes.top()};

  scope[token.m_lexeme] = true;
}

void Resolver::resolve_local(Expr &expr, const Token &name) {
  if (m_scopes.empty())
    return;

  auto scopes_copy{m_scopes};
  int depth{0};

  while (!scopes_copy.empty()) {
    if (scopes_copy.top().contains(name.m_lexeme)) {
      m_interpreter.resolve(expr, depth);
      return;
    }
    scopes_copy.pop();
    depth++;
  }
}

void Resolver::resolve_function(Function &function) {
  begin_scope();
  for (auto &param : function.m_parameters) {
    declare(param);
    define(param);
  }
  resolve(function.m_body);
  end_scope();
}

void Resolver::resolve_function(Anonymous &function) {
  begin_scope();
  for (auto &param : function.m_parameters) {
    declare(param);
    define(param);
  }
  resolve(function.m_body);
  end_scope();
}
