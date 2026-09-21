#pragma once

#include "../token/literal.hpp"
#include "lox_callable.hpp"
#include "lox_function.hpp"
#include <map>
#include <memory>
#include <string>

class LoxClass : public LoxCallable {
public:
  LoxClass(std::string &name,
           std::map<std::string, std::shared_ptr<LoxFunction>> methods)
      : m_name{name}, m_methods{std::move(methods)} {}
  operator std::string() const { return m_name; }

  std::strong_ordering operator<=>(const LoxClass &other) const {
    return m_name <=> other.m_name;
  }

  Object call(Interpreter &interpreter,
              std::vector<Object> &arguments) override;

  int arity() const override;

  std::shared_ptr<LoxFunction> find_method(std::string &name);

  std::string m_name{};
  std::map<std::string, std::shared_ptr<LoxFunction>> m_methods{};
};
