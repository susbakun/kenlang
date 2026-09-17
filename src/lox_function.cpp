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

  for (std::size_t i{}; i < m_declration->m_parameters.size(); i++) {
    environment.define(m_declration->m_parameters[i].m_lexeme, arguments[i]);
  }

  try {
    interpreter.execute_block(m_declration->m_body,
                              std::make_shared<Environment>(environment));
  } catch (ReturnException &exception) {
    return exception.m_value;
  }

  return std::monostate{};
}

int LoxFunction::arity() const { return m_declration->m_parameters.size(); }
