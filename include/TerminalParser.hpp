#pragma once

#include "TerminalExpr.hpp"
#include <string>

class TerminalParser {
public:
  explicit TerminalParser(std::string_view input);

  std::shared_ptr<Expr> parse();

private:
  std::shared_ptr<Expr> parseExpression();
  std::shared_ptr<Expr> parseArgument();
  std::shared_ptr<Expr> parsePrimary();
  std::shared_ptr<Expr> parsePostfix(std::shared_ptr<Expr> base);
  std::shared_ptr<Expr> parseAtom();
  std::string parseStringLiteral();

  [[nodiscard]] char peek() const;
  char advance();
  bool match(char c);

  std::string parseIdentifier();

private:
  std::string_view src;
  size_t pos = 0;
};