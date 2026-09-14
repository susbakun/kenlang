#pragma once

#include "literal.hpp"
#include "lox_callable.hpp"

class Clock : public LoxCallable {
public:
  Clock() = default;

  int arity() override;

  Object call(Interpreter &interpreter,
              std::vector<Object> &arguments) override;
};
