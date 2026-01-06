#pragma once

#include "TerminalCommands.hpp"

struct Expr {
  virtual ~Expr() = default;
  virtual TerminalCommands::RuntimeValue eval(Terminal &terminal) = 0;
};

struct ParameterExpr : Expr {
  std::string value;

  explicit ParameterExpr(std::string v) : value(std::move(v)) {}

  TerminalCommands::RuntimeValue eval(Terminal &terminal) override;
};

struct CommandExpr : Expr {
  std::string name;
  std::vector<std::shared_ptr<Expr>> args;

  CommandExpr(std::string name, std::vector<std::shared_ptr<Expr>> args)
      : name(std::move(name)), args(std::move(args)) {}

  TerminalCommands::RuntimeValue eval(Terminal &terminal) override;
};

struct DerefExpr : Expr {
  std::shared_ptr<Expr> inner;

  explicit DerefExpr(std::shared_ptr<Expr> inner) : inner(std::move(inner)) {}

  TerminalCommands::RuntimeValue eval(Terminal &terminal) override;
};

struct MemberExpr : Expr {
  std::shared_ptr<Expr> base;
  std::string member;

  MemberExpr(std::shared_ptr<Expr> base, std::string member)
      : base(std::move(base)), member(std::move(member)) {}

  TerminalCommands::RuntimeValue eval(Terminal &terminal) override;
};

struct AssignmentExpr : Expr {
  std::shared_ptr<Expr> lhs;
  std::shared_ptr<Expr> rhs;

  AssignmentExpr(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r)
      : lhs(std::move(l)), rhs(std::move(r)) {}

  TerminalCommands::RuntimeValue eval(Terminal &terminal) override;
};

struct StringExpr : Expr {
  std::string value;

  explicit StringExpr(std::string v) : value(std::move(v)) {}

  TerminalCommands::RuntimeValue eval(Terminal &) override;
};

struct BlockExpr : Expr {
  std::shared_ptr<Expr> inner;

  explicit BlockExpr(std::shared_ptr<Expr> inner) : inner(std::move(inner)) {}

  TerminalCommands::RuntimeValue eval(Terminal &) override;
};