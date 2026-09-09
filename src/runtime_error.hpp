#pragma once

#include "token.hpp"
#include <stdexcept>
#include <string>

class RuntimeError : public std::runtime_error {
public:
  RuntimeError(const Token &token, const std::string message)
      : m_token{token}, std::runtime_error{message} {}

  const Token &get_token() const { return m_token; }

private:
  Token m_token;
};
