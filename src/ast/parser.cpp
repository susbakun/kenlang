#include "parser.hpp"
#include "../lox.hpp"
#include "../token/token.hpp"
#include "../token/tokentype.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <sys/types.h>
#include <variant>
#include <vector>

std::unique_ptr<Expr> Parser::expression() { return comma(); }

std::unique_ptr<Expr> Parser::comma() {
  auto expr{assignment()};

  while (match({COMMA})) {
    auto op{previous()};
    auto right{assignment()};

    expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::assignment() {
  auto expr{ternary()};

  if (match({EQUAL})) {
    auto equals{previous()};
    auto value{assignment()};

    if (auto var = dynamic_cast<Var *>(expr.get())) {
      auto name{var->m_name};
      return std::make_unique<Assign>(name, std::move(value));
    } else if (auto var = dynamic_cast<Get *>(expr.get())) {
      auto name{var->m_name};
      return std::make_unique<Set>(std::move(var->m_obj), name,
                                   std::move(value));
    }

    error(equals, "Invalid assignment target.");
  }

  return expr;
}

std::unique_ptr<Expr> Parser::ternary() {
  auto expr{orexp()};

  if (match({QUESTION_MARK})) {
    auto left_operator{previous()};
    auto mid{orexp()};
    auto right_operator{consume(COLON, "Unexped token")};
    auto right{ternary()};

    expr = std::make_unique<Ternary>(std::move(expr), left_operator,
                                     std::move(mid), right_operator,
                                     std::move(right));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::orexp() {
  auto expr{andexp()};

  while (match({OR})) {
    auto op{previous()};
    auto right{andexp()};
    expr = std::make_unique<Logical>(std::move(expr), op, std::move(right));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::andexp() {
  auto expr{equality()};

  while (match({AND})) {
    auto op{previous()};
    auto right{equality()};
    expr = std::make_unique<Logical>(std::move(expr), op, std::move(right));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::equality() {
  auto expr{comparison()};

  while (match({BANG_EQUAL, EQUAL_EQUAL})) {
    auto op{previous()};
    auto right{comparison()};

    expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
  auto expr{term()};

  while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    auto op{previous()};
    auto right{term()};

    expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::term() {
  auto expr{factor()};

  while (match({MINUS, PLUS})) {
    auto op{previous()};
    auto right{factor()};

    expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::factor() {
  auto expr{unary()};

  while (match({STAR, SLASH})) {
    auto op{previous()};
    auto right{unary()};

    expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
  }

  return expr;
}

std::unique_ptr<Expr> Parser::unary() {
  if (match({BANG, MINUS})) {
    auto op{previous()};
    auto right{call()};

    return std::make_unique<Unary>(op, std::move(right));
  }

  return call();
}

std::unique_ptr<Expr> Parser::call() {
  auto expr{primary()};

  while (true) {
    if (match({LEFT_PAREN})) {
      expr = finish_call(std::move(expr));
    } else if (match({DOT})) {
      auto name{consume(IDENTIFIER, "Expect a name after '.'")};
      expr = std::make_unique<Get>(std::move(expr), name);
    } else {
      break;
    }
  }

  return expr;
}

std::unique_ptr<Expr> Parser::finish_call(std::unique_ptr<Expr> callee) {
  std::vector<std::unique_ptr<Expr>> arguments{};

  if (!check(RIGHT_PAREN)) {
    do {
      // limiting the number of arguments to 255
      if (arguments.size() >= 255) {
        error(peek(), "Can't have more than 255 arguments.");
      }
      arguments.push_back(assignment());
    } while (match({COMMA}));
  }

  auto paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");

  return std::make_unique<Call>(std::move(callee), paren, std::move(arguments));
}

std::unique_ptr<Expr> Parser::primary() {
  if (match({FALSE}))
    return std::make_unique<Literal>(false);
  if (match({TRUE}))
    return std::make_unique<Literal>(true);
  if (match({NIL}))
    return std::make_unique<Literal>(std::monostate{});
  if (match({FUN}))
    return anonymous_function();

  if (match({NUMBER, STRING})) {
    return std::make_unique<Literal>(previous().m_literal);
  }

  if (match({SUPER})) {
    auto keyword{previous()};
    consume(DOT, "Expected '.' after 'super'.");
    auto method{consume(IDENTIFIER, "Expect super class method name.")};
    return std::make_unique<Super>(keyword, method);
  }

  if (match({THIS})) {
    return std::make_unique<This>(previous());
  }

  if (match({IDENTIFIER})) {
    return std::make_unique<Var>(previous());
  }

  if (match({LEFT_PAREN})) {
    auto expr{expression()};
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_unique<Grouping>(std::move(expr));
  }

  throw error(peek(), "unexpected token");
}

std::unique_ptr<Expr> Parser::anonymous_function() {
  consume(LEFT_PAREN, "Expect '(' after anonymous function.");

  std::vector<Token> parameters{};
  if (!check(RIGHT_PAREN)) {
    do {
      if (parameters.size() >= 255) {
        error(peek(), "Can't have more than 255 parameters.");
      }
      parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
    } while (match({COMMA}));
  }

  consume(RIGHT_PAREN, "Expect ')' after parameters.");

  consume(LEFT_BRACE, "Expect '{' before anonymous function's body");
  auto body{block()};
  return std::make_unique<Anonymous>(std::move(parameters), std::move(body));
}

std::unique_ptr<Stmt> Parser::declaration() {
  try {
    if (match({CLASS}))
      return class_declration();
    // we need to exlude the anonymous functions here
    if (check(FUN) && check_next(IDENTIFIER))
      return function("function");
    if (match({VAR}))
      return var_declaration();

    return statement();

  } catch (ParseError &error) {
    synchronize();
    return nullptr;
  }
}

std::unique_ptr<Class> Parser::class_declration() {
  auto name{consume(IDENTIFIER, "Expect class name")};

  std::unique_ptr<Var> superclass{nullptr};
  if (match({LESS})) {
    auto name{consume(IDENTIFIER, "Expected a name after '<'.")};
    superclass = std::make_unique<Var>(name);
  }

  consume(LEFT_BRACE, "Expect '{' before class body");

  std::vector<std::shared_ptr<Function>> methods{};
  std::vector<std::shared_ptr<Function>> static_methods{};

  while (!check(RIGHT_BRACE) && !is_at_end()) {
    auto is_static{match({CLASS})};
    auto method{function("method")};

    if (is_static)
      static_methods.push_back(std::move(method));
    else
      methods.push_back(std::move(method));
  }

  consume(RIGHT_BRACE, "Expect '}' after class body");

  return std::make_unique<Class>(name, std::move(superclass),
                                 std::move(methods), std::move(static_methods));
}

std::unique_ptr<Function> Parser::function(std::string_view kind) {
  auto name{consume(IDENTIFIER, "Expect " + std::string(kind) + " name.")};

  bool is_getter{false};
  std::vector<Token> parameters{};

  if (kind == "method" && check(LEFT_BRACE)) {
    is_getter = true;
  } else {
    consume(LEFT_PAREN, "Expect '(' after " + std::string(kind) + " name.");
    if (!check(RIGHT_PAREN)) {
      do {
        if (parameters.size() >= 255) {
          error(peek(), "Can't have more than 255 parameters.");
        }
        parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
      } while (match({COMMA}));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");
  }
  consume(LEFT_BRACE, "Expect '{' before " + std::string(kind) + " body.");
  auto body{block()};
  return std::make_unique<Function>(name, std::move(parameters),
                                    std::move(body), is_getter);
}

std::unique_ptr<Variable> Parser::var_declaration() {
  auto name{consume(IDENTIFIER, "Expect variable name")};

  std::unique_ptr<Expr> initilizer{nullptr};
  if (match({EQUAL}))
    initilizer = expression();

  consume(SEMICOLON, "Expect ';' after variable declration.");
  return std::make_unique<Variable>(name, std::move(initilizer));
}

std::unique_ptr<Stmt> Parser::statement() {
  if (match({FOR}))
    return for_statement();
  if (match({IF}))
    return if_statement();
  if (match({PRINT}))
    return print_statement();
  if (match({RETURN}))
    return return_statement();
  if (match({WHILE}))
    return while_statement();
  if (match({BREAK}))
    return break_statement();
  if (match({LEFT_BRACE}))
    return std::make_unique<Block>(block());
  return expression_statement();
}

std::unique_ptr<Stmt> Parser::for_statement() {
  consume(LEFT_PAREN, "Expected '(' after 'for'.");

  std::unique_ptr<Stmt> initilizer{};
  if (match({SEMICOLON})) {
    initilizer = nullptr;
  } else if (match({VAR})) {
    initilizer = var_declaration();
  } else {
    initilizer = expression_statement();
  }

  std::unique_ptr<Expr> condition{};
  if (!check(SEMICOLON)) {
    condition = expression();
  }

  consume(SEMICOLON, "Expect ';' after loop condition");

  std::unique_ptr<Expr> increment{};
  if (!check(RIGHT_PAREN)) {
    increment = expression();
  }

  consume(RIGHT_PAREN, "Expect ')' after for clause.");

  m_loop_depth++;
  std::unique_ptr<Stmt> body{statement()};
  m_loop_depth--;

  if (initilizer != nullptr) {
    auto new_body{std::vector<std::unique_ptr<Stmt>>()};
    new_body.push_back(std::move(body));
    new_body.push_back(
        std::move(std::make_unique<Expression>(std::move(increment))));

    body = std::make_unique<Block>(std::move(new_body));
  }

  if (condition == nullptr)
    condition = std::make_unique<Literal>(true);
  body = std::make_unique<While>(std::move(condition), std::move(body));

  if (initilizer != nullptr) {
    auto new_body{std::vector<std::unique_ptr<Stmt>>()};
    new_body.push_back(std::move(initilizer));
    new_body.push_back(std::move(body));

    body = std::make_unique<Block>(std::move(new_body));
  }

  return body;
}

std::unique_ptr<If> Parser::if_statement() {
  consume(LEFT_PAREN, "Expect '(' after 'if'.");
  auto condition{expression()};
  consume(RIGHT_PAREN, "Expect ')' after if condition.");
  auto then_branch{statement()};
  std::unique_ptr<Stmt> else_branch{nullptr};

  if (match({ELSE})) {
    else_branch = statement();
  }

  return std::make_unique<If>(std::move(condition), std::move(then_branch),
                              std::move(else_branch));
}

std::unique_ptr<Print> Parser::print_statement() {
  auto value{expression()};
  consume(SEMICOLON, "Expected ; after a statement");
  return std::make_unique<Print>(std::move(value));
}

std::unique_ptr<Return> Parser::return_statement() {
  auto keyword{previous()};
  std::unique_ptr<Expr> value{nullptr};

  if (!check(SEMICOLON)) {
    value = expression();
  }

  consume(SEMICOLON, "Expected ';' after return statement");
  return std::make_unique<Return>(keyword, std::move(value));
}

std::unique_ptr<While> Parser::while_statement() {
  consume(LEFT_PAREN, "Expect '(' after 'while'.");
  auto condition{expression()};
  consume(RIGHT_PAREN, "Expect ')' after while condition.");
  m_loop_depth++;
  auto body{statement()};
  m_loop_depth--;

  return std::make_unique<While>(std::move(condition), std::move(body));
}

std::unique_ptr<Break> Parser::break_statement() {
  auto keyword{previous()};

  // check if we're inside a loop
  if (m_loop_depth == 0)
    error(keyword, "Cannot use 'break' outside of a loop.");

  consume(SEMICOLON, "Expected ';' after break keyword.");
  return std::make_unique<Break>(keyword);
}

std::vector<std::unique_ptr<Stmt>> Parser::block() {
  std::vector<std::unique_ptr<Stmt>> statements{};

  while (!check(RIGHT_BRACE) && !is_at_end()) {
    statements.push_back(declaration());
  }

  consume(RIGHT_BRACE, "Expect '}' after block");
  return statements;
}

std::unique_ptr<Expression> Parser::expression_statement() {
  auto value{expression()};
  consume(SEMICOLON, "Expected ; after a statement");
  return std::make_unique<Expression>(std::move(value));
}

bool Parser::match(std::initializer_list<TokenType> types) {
  for (const auto &type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }

  return false;
}

Token Parser::consume(TokenType type, std::string_view message) {
  if (check(type))
    return advance();
  throw error(peek(), message);
}

bool Parser::check(TokenType type) {
  if (is_at_end())
    return false;

  return (peek().m_type == type);
}

bool Parser::check_next(TokenType type) {
  if (is_at_end())
    return false;

  // consume the previous token
  advance();

  return check(type);
}

Token Parser::advance() {
  if (!is_at_end())
    m_current++;
  return previous();
}

Token Parser::previous() { return m_tokens[m_current - 1]; }

ParseError Parser::error(const Token &token, std::string_view message) {
  Lox::error(token, message);
  return ParseError{};
}

void Parser::synchronize() {
  advance();

  while (!is_at_end()) {
    if (previous().m_type == SEMICOLON)
      return;

    switch (peek().m_type) {
    case CLASS:
    case FUN:
    case VAR:
    case FOR:
    case IF:
    case WHILE:
    case PRINT:
    case RETURN:
      return;
    default:
    }

    advance();
  }
}

Token Parser::peek() { return m_tokens[m_current]; }

bool Parser::is_at_end() { return peek().m_type == EOFILE; }

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
  std::vector<std::unique_ptr<Stmt>> statements{};

  while (!is_at_end())
    statements.push_back(declaration());

  return statements;
}
