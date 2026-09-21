#pragma once

#include "../token/literal.hpp"
#include "../token/token.hpp"
#include <map>
#include <memory>
#include <string>

class Environment {
public:
  Environment() : m_enclosing{nullptr} {}

  explicit Environment(std::shared_ptr<Environment> enclosing)
      : m_enclosing{std::move(enclosing)} {}

  Object get(const Token &name) const;
  Object get_at(int distance, const std::string &name);
  std::shared_ptr<Environment> ancestor(int distance);
  void define(const std::string &name, const Object &value);
  void assign(const Token &name, const Object &value);
  void assign_at(int distance, const Token &name, Object &value);

private:
  std::shared_ptr<Environment> m_enclosing;
  std::map<std::string, Object> m_map{};
};
