#pragma once

#include <memory>
#include <string>

class LoxClass;

class LoxInstance {
public:
  LoxInstance(std::shared_ptr<LoxClass> klass);

  operator std::string() const;

private:
  std::shared_ptr<LoxClass> m_klass;
};
