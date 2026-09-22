#include "lox_function.hpp"
#include "../exceptions/return_exception.hpp"
#include "../execute/environment.hpp"
#include "../execute/interpreter.hpp"
#include "../token/literal.hpp"
#include "lox_instance.hpp"
#include <cstddef>
#include <memory>
#include <variant>

Object LoxFunction::call(Interpreter &interpreter,
                         std::vector<Object> &arguments) {
  Environment environment{m_closure};

  std::visit(
      [&](const auto &declration) {
        for (std::size_t i{}; i < declration->m_parameters.size(); i++) {
          environment.define(declration->m_parameters[i].m_lexeme,
                             arguments[i]);
        }
      },
      m_declration);

  try {
    std::visit(
        [&](const auto &declration) {
          interpreter.execute_block(declration->m_body,
                                    std::make_shared<Environment>(environment));
        },
        m_declration);
  } catch (ReturnException &exception) {
    if (m_is_initilizer)
      return m_closure->get_at(0, "this");

    return exception.m_value;
  }

  if (m_is_initilizer)
    return m_closure->get_at(0, "this");

  return std::monostate{};
}

LoxFunction LoxFunction::bind(const std::shared_ptr<LoxInstance> instance) {
  Environment environment{m_closure};
  environment.define("this", instance);

  return LoxFunction{m_declration, std::make_shared<Environment>(environment),
                     m_is_initilizer};
}

int LoxFunction::arity() const {
  return std::visit(
      [&](const auto &declration) -> std::size_t {
        return declration->m_parameters.size();
      },
      m_declration);
};
