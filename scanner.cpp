#include "scanner.hpp"
#include "lox.hpp"
#include "token.hpp"
#include "tokentype.hpp"
#include <cctype>
#include <variant>

std::vector<Token> Scanner::scan_tokens() {
  while (!is_at_end()) {
    m_start = m_current;

    scan_token();
  }

  m_tokens.emplace_back(EOFILE, "", std::monostate{}, m_line);

  return m_tokens;
}

void Scanner::scan_token() {
  char c{advance()};

  switch (c) {
  case '(':
    add_token(LEFT_PAREN);
    break;
  case ')':
    add_token(RIGHT_PAREN);
    break;
  case '{':
    add_token(LEFT_BRACE);
    break;
  case '}':
    add_token(RIGHT_BRACE);
    break;
  case ',':
    add_token(COMMA);
    break;
  case '.':
    add_token(DOT);
    break;
  case '-':
    add_token(MINUS);
    break;
  case '+':
    add_token(PLUS);
    break;
  case ';':
    add_token(SEMICOLON);
    break;
  case '*':
    add_token(STAR);
    break;
  case '!':
    add_token(match('=') ? BANG_EQUAL : BANG);
    break;
  case '=':
    add_token(match('=') ? EQUAL_EQUAL : EQUAL);
    break;
  case '<':
    add_token(match('=') ? LESS_EQUAL : LESS);
    break;
  case '>':
    add_token(match('=') ? GREATER_EQUAL : GREATER);
    break;
  case '/':
    if (match('/')) {
      while ((peek() != '\n') && !is_at_end())
        advance();
      // this only works on file mode
    } else if (match('*')) {
      int comment_count{1};
      while ((comment_count != 0) && !is_at_end()) {
        if (peek() == '\n')
          m_line++;
        if (match('/') && match('*')) {
          comment_count++;
        } else if (match('*') && match('/')) {
          comment_count--;
        } else {
          advance();
        }
      }

      if (is_at_end()) {
        Lox::error(m_line, "Unterminated comment");
        return;
      }
    } else {
      add_token(SLASH);
    }
    break;
  // ignore whitespace
  case ' ':
  case '\t':
  case '\r':
    break;
  case '\n':
    m_line++;
    break;
  case '"': {
    string();
    break;
  }

  default:
    if (std::isdigit(c)) {
      number();
    } else if (std::isalnum(c)) {
      identifier();
    } else {
      Lox::error(m_line, "Unexpected character.");
    }
    break;
  }
}

void Scanner::identifier() {
  while (std::isalnum(peek()))
    advance();

  auto text{m_source.substr(m_start, m_current - m_start)};
  auto keyword{m_keywords.find(text)};

  TokenType type{};

  // the keyword is not reserved
  // so it's an identifier
  if (keyword == m_keywords.end())
    type = IDENTIFIER;
  else
    type = keyword->second;

  add_token(type);
}

void Scanner::string() {
  while ((peek() != '"') && !is_at_end()) {
    // handling multi-line strings
    if (peek() == '\n')
      m_line++;

    advance();
  }

  if (is_at_end()) {
    Lox::error(m_line, "Unterminated string");
    return;
  }

  // skip the trailing "
  advance();

  auto value{m_source.substr(m_start + 1, m_current - (m_start + 3))};
  add_token(STRING, value);
}

void Scanner::number() {
  while (std::isdigit(peek())) {
    advance();
  }

  if ((peek() == '.') && std::isdigit(peek_next())) {
    while (std::isdigit(peek()))
      advance();
  }

  double value{std::stod(m_source.substr(m_start, m_current - m_start))};

  add_token(NUMBER, value);
}

bool Scanner::match(char expected) {
  if (is_at_end())
    return false;

  if (m_source[m_current] != expected)
    return false;

  m_current++;
  return true;
}

char Scanner::peek() {
  if (is_at_end())
    return '\0';

  return m_source[m_current];
}

char Scanner::peek_next() {
  if (m_current + 1 >= m_source.length())
    return '\0';

  return m_source[m_current + 1];
}

char Scanner::advance() { return m_source[m_current++]; }

void Scanner::add_token(TokenType type) { add_token(type, std::monostate()); }

void Scanner::add_token(TokenType type, Object literal) {
  auto lexeme{m_source.substr(m_start, m_current - m_start)};

  m_tokens.emplace_back(type, lexeme, literal, m_line);
}

bool Scanner::is_at_end() const { return m_current >= m_source.length(); }
