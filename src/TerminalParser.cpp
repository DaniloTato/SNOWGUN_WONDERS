#include "TerminalParser.hpp"
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
  auto lhs = parsePostfix(parsePrimary());

  while (true) {
    while (peek() == ' ')
      advance();

    if (match('=')) {
      auto rhs = parseExpression(); // right-associative
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

  if (match('*')) {
    return std::make_shared<DerefExpr>(parseAtom());
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

  if (peek() == '"' || peek() == '*' || peek() == '(' || peek() == '{')
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