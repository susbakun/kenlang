#include "lox_instance.hpp"
#include "../errors/runtime_error.hpp"
#include "../token/literal.hpp"
#include "lox_class.hpp"

LoxInstance::LoxInstance(std::shared_ptr<LoxClass> klass) : m_klass{klass} {}

std::string LoxInstance::to_string() const {
  return m_klass->m_name + " instance";
}

Object LoxInstance::get(Token &name) const {
  if (m_fields.contains(name)) {
    return m_fields.at(name);
  }

  throw RuntimeError{name, "Undefined property '" + name.m_lexeme + "'."};
}

void LoxInstance::set(Token &name, Object value) { m_fields[name] = value; }
