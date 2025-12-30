#pragma once

#include "GameObject.hpp"
#include "Scripter.hpp"

class ScriptRunner : public GameObject {
public:
  ScriptRunner();
  void update(const GeneralContext &ctx) override;
  Scripter<ScriptRunner> scripter;
};