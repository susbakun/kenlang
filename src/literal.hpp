#pragma once

#include <memory>
#include <string>
#include <variant>

class LoxCallable;
class LoxInstance;

using Object =
    std::variant<std::monostate, double, std::string, bool,
                 std::shared_ptr<LoxCallable>, std::shared_ptr<LoxInstance>>;

std::string literal_to_string(const Object &literal);
