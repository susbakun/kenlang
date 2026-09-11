#pragma once

#include "literal.hpp"
#include "token.hpp"
#include <map>
#include <string>

class Environment {
public:
  Environment() : m_enclosing{nullptr} {}

  Environment(Environment *enclosing) : m_enclosing{enclosing} {}

  Object get(const Token &name) const;
  void define(const std::string &name, const Object &value);
  void assign(const Token &name, const Object &value);

private:
  Environment *m_enclosing;
  std::map<std::string, Object> m_map{};
};
