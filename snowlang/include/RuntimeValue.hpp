#pragma once
#include "LambdaInstance.hpp"
#include "ObjectInstance.hpp"
#include <memory>
#include <variant>
#include <vector>

struct RExpr;
using RExprPtr = std::shared_ptr<RExpr>;

struct RStmt;
using RStmtPtr = std::shared_ptr<RStmt>;

struct RuntimeValue {

  struct Lambda {
    std::shared_ptr<LambdaInstance> instance;
    Lambda(std::shared_ptr<LambdaInstance> instance) : instance(std::move(instance)) {}
    Lambda() : instance(std::make_shared<LambdaInstance>(LambdaInstance{0, {}, {}, 0, {}})) {}
  };

  using List = std::vector<RuntimeValue>;
  using Null = std::monostate;
  using RuntimeType =
      std::variant<Null, float, bool, std::string, List, Lambda, ObjectRef, CellPtr>;
  RuntimeType data;

  RuntimeValue() : data(Null{}) {}

  RuntimeValue(float v) : data(v) {}
  RuntimeValue(bool v) : data(v) {}
  RuntimeValue(const std::string &v) : data(v) {}
  RuntimeValue(const List &v) : data(v) {}
  RuntimeValue(const Lambda &v) : data(v) {}
  RuntimeValue(const CellPtr &v) : data(v) {}
  RuntimeValue(const ObjectRef &v) : data(v) {}

  [[nodiscard]] std::string toString(bool showDataTypes = false) const;
};

using ValuePtr = std::shared_ptr<RuntimeValue>;