#pragma once

#include "../token/literal.hpp"
#include "lox_callable.hpp"
#include "lox_function.hpp"
#include "lox_instance.hpp"
#include <map>
#include <memory>
#include <string>

class LoxClass : public LoxCallable {
public:
  LoxClass(std::string &name,
           std::map<std::string, std::shared_ptr<LoxFunction>> methods,
           std::map<std::string, std::shared_ptr<LoxFunction>> static_methods)
      : m_name{name}, m_methods{std::move(methods)},
        m_static_methods{std::move(static_methods)} {}

  operator std::string() const { return m_name; }
  std::strong_ordering operator<=>(const LoxClass &other) const {
    return m_name <=> other.m_name;
  }

  Object call(Interpreter &interpreter,
              std::vector<Object> &arguments) override;

  int arity() const override;

  std::shared_ptr<LoxFunction> find_method(const std::string &name) const;
  std::shared_ptr<LoxFunction>
  find_static_method(const std::string &name) const;

  std::string m_name{};
  std::map<std::string, std::shared_ptr<LoxFunction>> m_methods{};
  std::map<std::string, std::shared_ptr<LoxFunction>> m_static_methods{};
};
