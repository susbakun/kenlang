#pragma once

#include "../ast/expression.hpp"
#include "../ast/statement.hpp"
#include "../execute/interpreter.hpp"
#include "../token/literal.hpp"
#include "../token/token.hpp"
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

enum class FunctionType { NONE, FUNCTION, METHOD };

struct VariableState {
  bool defined;
  bool used;
  int line_defined;
};

class Resolver : public ExprVisitor, StmtVisitor {
public:
  Resolver(Interpreter &interpreter) : m_interpreter{interpreter} {}

  Object visit_variable_expr(Var &expr) override;
  Object visit_assign_expr(Assign &expr) override;
  Object visit_binary_expr(Binary &expr) override;
  Object visit_ternary_expr(Ternary &expr) override;
  Object visit_call_expr(Call &expr) override;
  Object visit_grouping_expr(Grouping &expr) override;
  Object visit_literal_expr(Literal &expr) override;
  Object visit_logical_expr(Logical &expr) override;
  Object visit_unary_expr(Unary &expr) override;
  Object visit_anonymous_func_expr(Anonymous &expr) override;
  Object visit_get_expr(Get &expr) override;
  Object visit_set_expr(Set &expr) override;

  void visit_block_stmt(Block &stmt) override;
  void visit_var_stmt(Variable &stmt) override;
  void visit_function_stmt(Function &stmt) override;
  void visit_break_stmt(Break &stmt) override;
  void visit_continue_stmt(Continue &stmt) override;
  void visit_expression_stmt(Expression &stmt) override;
  void visit_if_stmt(If &stmt) override;
  void visit_print_stmt(Print &stmt) override;
  void visit_return_stmt(Return &stmt) override;
  void visit_while_stmt(While &stmt) override;
  void visit_class_stmt(Class &stmt) override;

  void resolve(const std::vector<std::unique_ptr<Stmt>> &statements);

private:
  void begin_scope();
  void resolve(Stmt &stmt);
  void resolve(Expr &expr);
  void end_scope();
  void declare(const Token &token);
  void define(const Token &token);
  void resolve_local(Expr &expr, const Token &name);
  void mark_as_used(const Token &name);

  template <typename T>
  void resolve_function(T &function, const FunctionType type);

  Interpreter &m_interpreter;
  std::stack<std::unordered_map<std::string, VariableState>> m_scopes{};
  FunctionType m_current_function{FunctionType::NONE};
};
