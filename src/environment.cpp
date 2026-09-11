#include "environment.hpp"
#include "literal.hpp"
#include "runtime_error.hpp"
#include "token.hpp"
#include <string>
#include <variant>

Object Environment::get(const Token &name) const {
  if (m_map.contains(name.m_lexeme)) {
    if (std::holds_alternative<std::monostate>(m_map.at(name.m_lexeme))) {
      throw RuntimeError{name, "Accessing uninitilzed variable '" +
                                   name.m_lexeme + "'."};
    }
    return m_map.at(name.m_lexeme);
  }

  if (m_enclosing != nullptr)
    return m_enclosing->get(name);

  throw RuntimeError{name, "Undefined variable '" + name.m_lexeme + "'."};
}

void Environment::assign(const Token &name, const Object &value) {
  if (m_map.contains(name.m_lexeme)) {
    m_map[name.m_lexeme] = value;
    return;
  }

  if (m_enclosing != nullptr)
    m_enclosing->assign(name, value);

  throw RuntimeError{name, "Undefined variable '" + name.m_lexeme + "'."};
}

void Environment::define(const std::string &name, const Object &value) {
  m_map[name] = value;
}
