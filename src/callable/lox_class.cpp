#include "lox_class.hpp"
#include "../token/literal.hpp"
#include "lox_function.hpp"
#include "lox_instance.hpp"
#include <memory>

Object LoxClass::call(Interpreter &interpreter,
                      std::vector<Object> &arguments) {
  auto instance{
      std::make_shared<LoxInstance>(std::make_shared<LoxClass>(*this))};

  auto init{find_method("init")};
  if (init != nullptr)
    init->bind(instance).call(interpreter, arguments);

  return instance;
}

std::shared_ptr<LoxFunction>
LoxClass::find_method(const std::string &name) const {
  if (m_methods.contains(name)) {
    return m_methods.at(name);
  }

  return nullptr;
}

int LoxClass::arity() const {
  auto init{find_method("init")};
  if (init != nullptr)
    return init->arity();

  return 0;
}
