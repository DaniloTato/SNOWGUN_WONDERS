#include "TerminalParser.hpp"
#include <iostream>
#include <memory>

TerminalParser::TerminalParser(std::string_view input) : src(input) {}

char TerminalParser::peek() const {
  if (pos >= src.size())
    return '\0';
  return src[pos];
}

char TerminalParser::advance() {
  if (pos >= src.size())
    return '\0';
  return src[pos++];
}

bool TerminalParser::match(char c) {
  if (peek() == c) {
    advance();
    return true;
  }
  return false;
}

std::string TerminalParser::parseStringLiteral() {
  std::string out;

  while (peek() != '"' && peek() != '\0') {
    out.push_back(advance());
  }

  if (!match('"')) {
    throw std::runtime_error("Unterminated string literal");
  }

  return out;
}

std::string TerminalParser::parseIdentifier() {
  std::string id;

  while (std::isalnum(peek()) || peek() == '_') {
    id.push_back(advance());
  }

  if (id.empty()) {
    throw std::runtime_error("Expected identifier");
  }

  return id;
}

std::shared_ptr<Expr> TerminalParser::parse() { return parseExpression(); }

std::shared_ptr<Expr> TerminalParser::parsePrimary() {
  while (peek() == ' ')
    advance();

  if (std::isalpha(peek()) || peek() == '_') {
    std::string name = parseIdentifier();
    std::vector<std::shared_ptr<Expr>> args;

    while (true) {
      while (peek() == ' ')
        advance();

      if (peek() == ')' || peek() == '}' || peek() == '\0')
        break;

      args.push_back(parseArgument());
    }

    return std::make_shared<CommandExpr>(name, std::move(args));
  }

  return parseAtom();
}

std::shared_ptr<Expr> TerminalParser::parseExpression() {
  auto lhs = parseComparison();

  while (true) {
    while (peek() == ' ')
      advance();

    if (match('=')) {
      auto rhs = parseExpression();
      lhs = std::make_shared<AssignmentExpr>(std::move(lhs), std::move(rhs));
    } else {
      break;
    }
  }

  return lhs;
}

std::shared_ptr<Expr> TerminalParser::parseAtom() {
  while (peek() == ' ')
    advance();

  if (match('"')) {
    return std::make_unique<StringExpr>(parseStringLiteral());
  }

  if (std::isdigit(peek())) {
    return std::make_shared<NumberExpr>(parseNumber());
  }

  if (match('*')) {
    return std::make_shared<DerefExpr>(parseAtom());
  }

  if (match('$')) {
    std::string name = parseIdentifier();

    auto param = std::make_shared<ParameterExpr>(name);
    return std::make_shared<ValueExpr>(param);
  }

  if (match('(')) {
    auto expr = parseExpression();

    while (peek() == ' ')
      advance();

    if (!match(')'))
      throw std::runtime_error("Expected ')'");

    return expr;
  }

  if (match('{')) {
    auto expr = parseExpression();

    while (peek() == ' ')
      advance();

    if (!match('}'))
      throw std::runtime_error("Expected '}'");

    return std::make_shared<BlockExpr>(std::move(expr));
  }

  throw std::runtime_error("Expected expression");
}

std::shared_ptr<Expr> TerminalParser::parseArgument() {
  while (peek() == ' ')
    advance();

  if (peek() == '"' || peek() == '*' || peek() == '$' || peek() == '(' ||
      peek() == '{')
    return parseAtom();

  std::string name = parseIdentifier();
  return std::make_unique<ParameterExpr>(name);
}

std::shared_ptr<Expr> TerminalParser::parsePostfix(std::shared_ptr<Expr> base) {
  while (true) {
    if (match('.')) {
      std::string member = parseIdentifier();
      base = std::make_shared<MemberExpr>(std::move(base), member);
    } else {
      break;
    }
  }
  return base;
}

bool TerminalParser::matchComparison(CompareOp &op) {
  if (match('=')) {
    if (match('=')) {
      op = CompareOp::Equal;
      return true;
    }
    pos--;
    return false;
  }

  if (match('!')) {
    if (match('=')) {
      op = CompareOp::NotEqual;
      return true;
    }
    throw std::runtime_error("Expected '=' after '!'");
  }

  if (match('<')) {
    if (match('=')) {
      op = CompareOp::LessEqual;
    } else {
      op = CompareOp::Less;
    }
    return true;
  }

  if (match('>')) {
    if (match('=')) {
      op = CompareOp::GreaterEqual;
    } else {
      op = CompareOp::Greater;
    }
    return true;
  }

  return false;
}

std::shared_ptr<Expr> TerminalParser::parseComparison() {
  auto lhs = parseAdditive();

  while (true) {
    while (peek() == ' ')
      advance();

    CompareOp op;
    if (!matchComparison(op))
      break;

    auto rhs = parseAdditive();
    lhs = std::make_shared<CompareExpr>(op, lhs, rhs);
  }

  return lhs;
}

std::shared_ptr<Expr> TerminalParser::parseAdditive() {
  auto lhs = parseMultiplicative();

  while (true) {
    while (peek() == ' ')
      advance();

    if (match('+')) {
      lhs = std::make_shared<MathExpr>(MathOp::Add, lhs, parseMultiplicative());
    } else if (match('-')) {
      lhs = std::make_shared<MathExpr>(MathOp::Sub, lhs, parseMultiplicative());
    } else {
      break;
    }
  }

  return lhs;
}

std::shared_ptr<Expr> TerminalParser::parseMultiplicative() {
  auto lhs = parseUnary();

  while (true) {
    while (peek() == ' ')
      advance();

    if (match('*')) {
      lhs = std::make_shared<MathExpr>(MathOp::Mul, lhs, parseUnary());
    } else if (match('/')) {
      lhs = std::make_shared<MathExpr>(MathOp::Div, lhs, parseUnary());
    } else {
      break;
    }
  }

  return lhs;
}

std::shared_ptr<Expr> TerminalParser::parseUnary() {
  while (peek() == ' ')
    advance();

  if (match('-')) {
    auto zero = std::make_shared<NumberExpr>(0.0);
    auto rhs = parseUnary();
    return std::make_shared<MathExpr>(MathOp::Sub, zero, rhs);
  }

  return parsePostfix(parsePrimary());
}

float TerminalParser::parseNumber() {
  std::string num;
  while (std::isdigit(peek()) || peek() == '.') {
    num.push_back(advance());
  }

  return std::stof(num);
}