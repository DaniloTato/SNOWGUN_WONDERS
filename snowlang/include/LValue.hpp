#pragma once

#include "Location.hpp"
#include "ObjectInstance.hpp"
#include "RuntimeValue.hpp"
#include "SnowlangInstance.hpp"
#include "Tokenizer.hpp"

#include <string>

namespace Snowlang {

struct LValue {
  enum class Kind : std::uint8_t { Variable, ObjectField, ListElement };

  Kind kind;

  Location location;
  ObjectRef object;
  std::string field;

  std::unique_ptr<LValue> base;
  int index = 0;

  void LValueWrite(RuntimeValue value, SnowlangInstance &snowlang);
  RuntimeValue LValueRead(SnowlangInstance &snowlang, const SourceSpan &span) const;
};

} // namespace Snowlang