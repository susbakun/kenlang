#pragma once

#include "../token/literal.hpp"

class ReturnException {
public:
  ReturnException(Object &value) : m_value{value} {}

  const Object m_value{};
};
