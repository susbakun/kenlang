#include "token.hpp"
#include "literal.hpp"
#include "tokentype.hpp"

Token::operator std::string() const {
  return static_cast<std::string>(get_tokentype_str(m_type)) + " " + m_lexeme +
         " " + literal_to_string(m_literal);
}
