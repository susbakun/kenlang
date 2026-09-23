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

  LoxFunction(Declaration declaration, std::shared_ptr<Environment> closure,
              bool is_initilizer, bool is_getter)
      : m_declration{std::move(declaration)}, m_closure{std::move(closure)},
        m_is_initilizer{is_initilizer}, m_is_getter{is_getter} {}

  Object call(Interpreter &interpreter,
              std::vector<Object> &arguments) override;

  LoxFunction bind(const std::shared_ptr<LoxInstance> instance);

  bool is_getter() const;

  int arity() const override;

private:
  Declaration m_declration;
  std::shared_ptr<Environment> m_closure{};
  bool m_is_initilizer{};
  bool m_is_getter{};
};
