#pragma once

#include "literal.hpp"
#include "lox_callable.hpp"
#include <string>

class LoxClass : public LoxCallable {
public:
  LoxClass(std::string &name) : m_name{name} {}
  operator std::string() const { return m_name; }

  std::strong_ordering operator<=>(const LoxClass &other) const {
    return m_name <=> other.m_name;
  }

  Object call(Interpreter &interpreter,
              std::vector<Object> &arguments) override;

  int arity() const override;

  std::string m_name{};
};
