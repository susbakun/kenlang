#pragma once

#include <sstream>
#include <string>
#include <variant>

using Object = std::variant<std::monostate, double, std::string, bool>;

inline std::string literal_to_string(const Object &literal) {
  return std::visit(
      [](const auto &value) -> std::string {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
          return "nil";
        } else if constexpr (std::is_same_v<T, std::string>) {
          return value;
        } else if constexpr (std::is_same_v<T, bool>) {
          return value ? "true" : "false";
        } else if constexpr (std::is_same_v<T, double>) {
          std::ostringstream out;
          out << value;
          return out.str();
        } else {
          std::to_string(value);
        }
      },
      literal);
}
