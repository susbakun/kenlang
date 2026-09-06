#pragma once

#include <string_view>

enum TokenType {
  // Single-character tokens
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // one or two character tokens
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // ternary
  QUESTION_MARK,
  COLON,

  // literals
  IDENTIFIER,
  STRING,
  NUMBER,

  // keywords
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  EOFILE

};

inline constexpr std::string_view get_tokentype_str(TokenType type) {
  switch (type) {
    // Single-character tokens
  case LEFT_PAREN:
    return "LEFT_PAREN";
  case RIGHT_PAREN:
    return "RIGHT_PAREN";
  case LEFT_BRACE:
    return "LEFT_BRACE";
  case RIGHT_BRACE:
    return "RIGHT_BRACE";
  case COMMA:
    return "COMMA";
  case DOT:
    return "DOT";
  case MINUS:
    return "MINUS";
  case PLUS:
    return "PLUS";
  case SEMICOLON:
    return "SEMICOLON";
  case SLASH:
    return "SLASH";
  case STAR:
    return "STAR";

    // one or two character tokens
  case BANG:
    return "BANG";
  case BANG_EQUAL:
    return "BANG_EQUAL";
  case EQUAL:
    return "EQUAL";
  case EQUAL_EQUAL:
    return "EQUAL_EQUAL";
  case GREATER:
    return "GREATER";
  case GREATER_EQUAL:
    return "GREATER_EQUAL";
  case LESS:
    return "LESS";
  case LESS_EQUAL:
    return "LESS_EQUAL";

    // literals
  case IDENTIFIER:
    return "IDENTIFIER";
  case STRING:
    return "STRING";
  case NUMBER:
    return "NUMBER";

    // keywords
  case AND:
    return "AND";
  case CLASS:
    return "CLASS";
  case ELSE:
    return "ELSE";
  case FALSE:
    return "FALSE";
  case FUN:
    return "FUN";
  case FOR:
    return "FOR";
  case IF:
    return "IF";
  case NIL:
    return "NIL";
  case OR:
    return "OR";
  case PRINT:
    return "PRINT";
  case RETURN:
    return "RETURN";
  case SUPER:
    return "SUPER";
  case THIS:
    return "THIS";
  case TRUE:
    return "TRUE";
  case VAR:
    return "VAR";
  case WHILE:
    return "WHILE";

  case EOFILE:
    return "EOFILE";

  default:
    return "UNKNOWN";
  }
}
