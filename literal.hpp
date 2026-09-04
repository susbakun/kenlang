#pragma once

#include <string>
#include <variant>

using Literal = std::variant<std::monostate, double, std::string, bool>;

inline std::string literal_to_string(const Literal &literal) {
  return std::visit(
      [](const auto &value) -> std::string {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
          return "nil";
        } else if constexpr (std::is_same_v<T, std::string>) {
          return value;
        } else if constexpr (std::is_same_v<T, bool>) {
          return value ? "true" : "false";
        } else {
          return std::to_string(value);
        }
      },
      literal);
}
