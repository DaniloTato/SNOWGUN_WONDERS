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

struct NumberExpr : Expr {
  float value;

  explicit NumberExpr(float v) : value(v) {}

  TerminalCommands::RuntimeValue eval(Terminal &) override;
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

struct ValueExpr : Expr {
  std::shared_ptr<Expr> inner;

  explicit ValueExpr(std::shared_ptr<Expr> inner) : inner(std::move(inner)) {}

  TerminalCommands::RuntimeValue eval(Terminal &) override;
};

enum class CompareOp : std::uint8_t {
  Equal,
  Greater,
  Less,
  GreaterEqual,
  LessEqual,
  NotEqual
};

struct CompareExpr : Expr {
  CompareOp op;
  std::shared_ptr<Expr> lhs;
  std::shared_ptr<Expr> rhs;

  CompareExpr(CompareOp op, std::shared_ptr<Expr> lhs,
              std::shared_ptr<Expr> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  TerminalCommands::RuntimeValue eval(Terminal &terminal) override;

private:
  bool runtimeEqual(const TerminalCommands::RuntimeValue &a,
                    const TerminalCommands::RuntimeValue &b);
};

enum class MathOp : std::uint8_t { Add, Sub, Mul, Div };

struct MathExpr : Expr {
  MathOp op;
  std::shared_ptr<Expr> lhs;
  std::shared_ptr<Expr> rhs;

  MathExpr(MathOp op, std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  TerminalCommands::RuntimeValue eval(Terminal &terminal) override;
};