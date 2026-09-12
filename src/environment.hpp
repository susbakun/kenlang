#pragma once

#include "literal.hpp"
#include "token.hpp"
#include <map>
#include <memory>
#include <string>

class Environment {
public:
  Environment() : m_enclosing{nullptr} {}

  explicit Environment(std::shared_ptr<Environment> enclosing)
      : m_enclosing{std::move(enclosing)} {}

  Object get(const Token &name) const;
  void define(const std::string &name, const Object &value);
  void assign(const Token &name, const Object &value);

private:
  std::shared_ptr<Environment> m_enclosing;
  std::map<std::string, Object> m_map{};
};
