#include "lox_function.hpp"
#include "environment.hpp"
#include "interpreter.hpp"
#include "literal.hpp"
#include <cstddef>
#include <variant>

Object LoxFunction::call(Interpreter &interpreter,
                         std::vector<Object> &arguments) {
  Environment environment{interpreter.m_globals};

  for (std::size_t i{}; i < m_declration->m_parameters.size(); i++) {
    environment.define(m_declration->m_parameters[i].m_lexeme, arguments[i]);
  }

  interpreter.execute_block(m_declration->m_body,
                            std::make_shared<Environment>(environment));

  return std::monostate{};
}

int LoxFunction::arity() const { return m_declration->m_parameters.size(); }
