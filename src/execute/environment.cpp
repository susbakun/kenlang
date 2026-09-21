#include "environment.hpp"
#include "../errors/runtime_error.hpp"
#include "../token/literal.hpp"
#include "../token/token.hpp"
#include <cstddef>
#include <memory>
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

Object Environment::get_at(int distance, const std::string &name) {
  return ancestor(distance)->m_map.at(name);
}

std::shared_ptr<Environment> Environment::ancestor(int distance) {
  auto environment{std::make_shared<Environment>(*this)};

  for (std::size_t i{}; i < distance; i++) {
    environment = environment->m_enclosing;
  }

  return environment;
}

void Environment::assign(const Token &name, const Object &value) {
  if (m_map.contains(name.m_lexeme)) {
    m_map[name.m_lexeme] = value;
    return;
  }

  if (m_enclosing != nullptr) {
    m_enclosing->assign(name, value);
    return;
  }

  throw RuntimeError{name, "Undefined variable '" + name.m_lexeme + "'."};
}

void Environment::assign_at(int distance, const Token &name, Object &value) {
  ancestor(distance)->assign(name, value);
}

void Environment::define(const std::string &name, const Object &value) {
  m_map[name] = value;
}
