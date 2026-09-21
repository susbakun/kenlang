
#include "clock.hpp"
#include <chrono>

int Clock::arity() const { return 0; }

Object Clock::call(Interpreter &interpreter, std::vector<Object> &arguments) {
  using namespace std::chrono;

  return duration<double>(system_clock::now().time_since_epoch()).count();
}
