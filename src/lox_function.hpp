#pragma once

#include "environment.hpp"
#include "literal.hpp"
#include "lox_callable.hpp"
#include "statement.hpp"
#include <memory>

class LoxFunction : public LoxCallable {
public:
  LoxFunction(std::shared_ptr<Function> declration,
              std::shared_ptr<Environment> closure)
      : m_declration{std::move(declration)}, m_closure{closure} {}

  Object call(Interpreter &interpreter,
              std::vector<Object> &arguments) override;

  int arity() const override;

private:
  std::shared_ptr<Function> m_declration;
  std::shared_ptr<Environment> m_closure{};
};
