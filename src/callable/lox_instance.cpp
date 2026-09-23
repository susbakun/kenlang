#include "lox_instance.hpp"
#include "../errors/runtime_error.hpp"
#include "../token/literal.hpp"
#include "lox_class.hpp"
#include "lox_function.hpp"
#include <memory>
#include <vector>

LoxInstance::LoxInstance(std::shared_ptr<LoxClass> klass) : m_klass{klass} {}

std::string LoxInstance::to_string() const {
  return m_klass->m_name + " instance";
}

Object LoxInstance::get(const Token &name, Interpreter &interpreter) {
  if (m_fields.contains(name)) {
    return m_fields.at(name);
  }

  auto method{m_klass->find_method(name.m_lexeme)};
  if (method != nullptr) {
    auto bound{method->bind(shared_from_this())};
    if (method->is_getter()) {
      std::vector<Object> arguments{};
      return bound.call(interpreter, arguments);
    }
    return std::make_shared<LoxFunction>(bound);
  }

  throw RuntimeError{name, "Undefined property '" + name.m_lexeme + "'."};
}

void LoxInstance::set(const Token &name, const Object value) {
  m_fields[name] = value;
}
