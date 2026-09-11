#pragma once

#include "literal.hpp"
#include "token.hpp"
#include <map>
#include <string>

class Environment {
public:
  Environment() = default;

  Object get(Token &name) const;

  void define(std::string &name, Object &value);

  std::map<std::string, Object> m_map{};
};
