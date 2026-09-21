#include "lox_class.hpp"
#include "literal.hpp"
#include "lox_instance.hpp"
#include <memory>

Object LoxClass::call(Interpreter &interpreter,
                      std::vector<Object> &arguments) {
  LoxInstance instance{std::make_shared<LoxClass>(*this)};
  return instance;
}

int LoxClass::arity() const { return 0; }
