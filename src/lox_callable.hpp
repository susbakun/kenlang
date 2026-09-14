#pragma once

#include "literal.hpp"
#include <vector>

class Interpreter; // forward-declared — call() only needs a reference

class LoxCallable {
public:
  virtual ~LoxCallable() = default;
  virtual Object call(Interpreter &interpreter,
                      std::vector<Object> &arguments) = 0;
  virtual int arity() = 0;
};
