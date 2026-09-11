#include "environment.hpp"
#include "literal.hpp"
#include "runtime_error.hpp"
#include "token.hpp"
#include <string>

Object Environment::get(const Token &name) const {
  if (m_map.contains(name.m_lexeme)) {
    return m_map.at(name.m_lexeme);
  }

  throw RuntimeError{name, "Undefined variable '" + name.m_lexeme + "'."};
}

void Environment::define(const std::string &name, const Object &value) {
  m_map.insert({name, value});
}
