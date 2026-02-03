#pragma once

#include "Tokenizer.hpp"

struct Node {
  SourceSpan span;
  virtual ~Node() = default;
};