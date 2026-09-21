#include "lox_instance.hpp"
#include "lox_class.hpp"

LoxInstance::LoxInstance(std::shared_ptr<LoxClass> klass) : m_klass{klass} {}

LoxInstance::operator std::string() const {
  return m_klass->m_name + " instance";
}
