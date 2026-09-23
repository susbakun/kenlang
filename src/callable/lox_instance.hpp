#pragma once

#include "../execute/interpreter.hpp"
#include "../token/literal.hpp"
#include "../token/token.hpp"
#include <map>
#include <memory>
#include <string>

class LoxClass;

class LoxInstance : public std::enable_shared_from_this<LoxInstance> {
public:
  LoxInstance(std::shared_ptr<LoxClass> klass);

  LoxInstance() = default;

  std::string to_string() const;

  Object get(Token &name, Interpreter &interpreter);
  void set(Token &name, Object value);

private:
  std::shared_ptr<LoxClass> m_klass{};
  std::map<std::string, Object> m_fields{};
};
