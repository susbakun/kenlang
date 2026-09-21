#pragma once

#include <string>
class LoxClass {
public:
  LoxClass(std::string &name) : m_name{name} {}

  operator std::string() const { return m_name; }

  bool operator<=>(const LoxClass &other) const = default;

  std::string m_name{};
};
