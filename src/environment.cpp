#include "environment.hpp"
#include "literal.hpp"
#include "runtime_error.hpp"
#include "token.hpp"
#include <string>

Object Environment::get(Token &name) const {
  if (m_map.contains(name)) {
    return m_map.at(name);
  }

  throw RuntimeError{name, "Undefined variable '" + name.m_lexeme + "'."};
}

void Environment::define(std::string &name, Object &value) {
  m_map.insert({name, value});
}
