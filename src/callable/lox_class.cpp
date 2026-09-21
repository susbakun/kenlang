#include "lox_class.hpp"
#include "../token/literal.hpp"
#include "lox_function.hpp"
#include "lox_instance.hpp"
#include <memory>

Object LoxClass::call(Interpreter &interpreter,
                      std::vector<Object> &arguments) {
  auto instance{
      std::make_shared<LoxInstance>(std::make_shared<LoxClass>(*this))};
  return instance;
}

std::shared_ptr<LoxFunction> LoxClass::find_method(std::string &name) {
  if (m_methods.contains(name)) {
    return m_methods[name];
  }

  return nullptr;
}

int LoxClass::arity() const { return 0; }
