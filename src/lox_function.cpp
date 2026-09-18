#include "lox_function.hpp"
#include "environment.hpp"
#include "interpreter.hpp"
#include "literal.hpp"
#include "return_exception.hpp"
#include <cstddef>
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
    return exception.m_value;
  }

  return std::monostate{};
}

int LoxFunction::arity() const {
  return std::visit(
      [&](const auto &declration) -> std::size_t {
        return declration->m_parameters.size();
      },
      m_declration);
};
