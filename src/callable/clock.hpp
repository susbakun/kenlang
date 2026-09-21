#pragma once

#include "../token/literal.hpp"
#include "lox_callable.hpp"

class Clock : public LoxCallable {
public:
  Clock() = default;

  int arity() const override;

  Object call(Interpreter &interpreter,
              std::vector<Object> &arguments) override;
};
