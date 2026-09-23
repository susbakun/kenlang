#pragma once

#include "../token/literal.hpp"
#include "../token/token.hpp"
#include <memory>
#include <vector>

class Binary;
class Grouping;
class Literal;
class Unary;
class Ternary;
class Var;
class Assign;
class Logical;
class Call;
class Anonymous;
class Get;
class Set;
class This;
class Super;

class Stmt;

struct ExprVisitor {
  virtual Object visit_binary_expr(Binary &expr) = 0;
  virtual Object visit_grouping_expr(Grouping &expr) = 0;
  virtual Object visit_literal_expr(Literal &expr) = 0;
  virtual Object visit_unary_expr(Unary &expr) = 0;
  virtual Object visit_ternary_expr(Ternary &expr) = 0;
  virtual Object visit_variable_expr(Var &expr) = 0;
  virtual Object visit_assign_expr(Assign &expr) = 0;
  virtual Object visit_logical_expr(Logical &expr) = 0;
  virtual Object visit_call_expr(Call &expr) = 0;
  virtual Object visit_anonymous_func_expr(Anonymous &expr) = 0;
  virtual Object visit_get_expr(Get &expr) = 0;
  virtual Object visit_set_expr(Set &expr) = 0;
  virtual Object visit_this_expr(This &expr) = 0;
  virtual Object visit_super_expr(Super &expr) = 0;

  virtual ~ExprVisitor() = default;
};

class Expr {
public:
  virtual ~Expr() = default;
  virtual Object accept(ExprVisitor &visitor) = 0;
};

class Binary : public Expr {
public:
  Binary(std::unique_ptr<Expr> left, Token &op, std::unique_ptr<Expr> right);

  Object accept(ExprVisitor &visitor) override;

  std::unique_ptr<Expr> m_left;
  const Token m_op;
  std::unique_ptr<Expr> m_right;
};

class Grouping : public Expr {
public:
  explicit Grouping(std::unique_ptr<Expr> expression);

  Object accept(ExprVisitor &visitor) override;

  std::unique_ptr<Expr> m_expression;
};

class Literal : public Expr {
public:
  explicit Literal(Object value);

  Object accept(ExprVisitor &visitor) override;

  const Object m_value{};
};

class Unary : public Expr {
public:
  Unary(const Token &op, std::unique_ptr<Expr> right);

  Object accept(ExprVisitor &visitor) override;

  const Token m_operator;
  std::unique_ptr<Expr> m_right;
};

class Ternary : public Expr {
public:
  Ternary(std::unique_ptr<Expr> left, const Token &left_operator,
          std::unique_ptr<Expr> mid, const Token &right_operator,
          std::unique_ptr<Expr> right);

  Object accept(ExprVisitor &visitor) override;

  std::unique_ptr<Expr> m_left;
  const Token m_left_operator;
  std::unique_ptr<Expr> m_mid;
  const Token m_right_operator;
  std::unique_ptr<Expr> m_right;
};

class Var : public Expr {
public:
  explicit Var(const Token &name);

  Object accept(ExprVisitor &visitor) override;

  const Token m_name;
};

class Assign : public Expr {
public:
  Assign(const Token &name, std::unique_ptr<Expr> value);

  Object accept(ExprVisitor &visitor) override;

  const Token m_name;
  std::unique_ptr<Expr> m_value{};
};

class Logical : public Expr {
public:
  Logical(std::unique_ptr<Expr> left, const Token &op,
          std::unique_ptr<Expr> right);

  Object accept(ExprVisitor &visitor) override;

  std::unique_ptr<Expr> m_left{};
  const Token m_operator;
  std::unique_ptr<Expr> m_right{};
};

class Call : public Expr {
public:
  Call(std::unique_ptr<Expr> callee, const Token &paren,
       std::vector<std::unique_ptr<Expr>> arguments);

  Object accept(ExprVisitor &visitor) override;

  std::unique_ptr<Expr> m_callee{};
  const Token m_paren;
  std::vector<std::unique_ptr<Expr>> m_arguments{};
};

class Anonymous : public Expr {
public:
  Anonymous(const std::vector<Token> parameters,
            const std::vector<std::unique_ptr<Stmt>> body);

  Anonymous(Anonymous &&) noexcept;

  ~Anonymous();

  Object accept(ExprVisitor &visitor) override;

  std::vector<Token> m_parameters{};
  std::vector<std::unique_ptr<Stmt>> m_body{};
};

class Get : public Expr {
public:
  Get(std::unique_ptr<Expr> obj, const Token &name);

  Object accept(ExprVisitor &visitor);

  std::unique_ptr<Expr> m_obj;
  const Token m_name;
};

class Set : public Expr {
public:
  Set(std::unique_ptr<Expr> obj, const Token &name,
      std::unique_ptr<Expr> value);

  Object accept(ExprVisitor &visitor) override;

  std::unique_ptr<Expr> m_obj;
  const Token m_name;
  std::unique_ptr<Expr> m_value;
};

class This : public Expr {
public:
  This(const Token &keyword);

  Object accept(ExprVisitor &visitor) override;

  const Token m_keyword;
};

class Super : public Expr {
public:
  Super(const Token &keyword, const Token &method);

  Object accept(ExprVisitor &visitor) override;

  const Token m_keyword;
  const Token m_method;
};
