#pragma once

#include <string>
#include <unordered_map>

struct RuntimeValue;

struct ObjectInstance {
  std::unordered_map<std::string, RuntimeValue> fields;
};

using ObjectRef = std::shared_ptr<ObjectInstance>;