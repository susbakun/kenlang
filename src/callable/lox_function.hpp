#pragma once

#include "../ast/expression.hpp"
#include "../ast/statement.hpp"
#include "../execute/environment.hpp"
#include "../token/literal.hpp"
#include "lox_callable.hpp"
#include <memory>
#include <variant>

class LoxFunction : public LoxCallable {
public:
  using Declaration =
      std::variant<std::shared_ptr<Function>, std::shared_ptr<Anonymous>>;

  LoxFunction(Declaration declaration, std::shared_ptr<Environment> closure)
      : m_declration{std::move(declaration)}, m_closure{std::move(closure)} {}

  LoxFunction(std::shared_ptr<Function> declration,
              std::shared_ptr<Environment> closure)
      : m_declration{std::move(declration)}, m_closure{closure} {}

  LoxFunction(std::shared_ptr<Anonymous> declration,
              std::shared_ptr<Environment> closure)
      : m_declration{std::move(declration)}, m_closure{closure} {}

  Object call(Interpreter &interpreter,
              std::vector<Object> &arguments) override;

  LoxFunction bind(const LoxInstance &instance);

  int arity() const override;

private:
  Declaration m_declration;
  std::shared_ptr<Environment> m_closure{};
};
