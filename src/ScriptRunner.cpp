#include "ScriptRunner.hpp"
#include "Scripter.hpp"

ScriptRunner::ScriptRunner() : GameObject() {}

void ScriptRunner::update(const GeneralContext &ctx) {
  scripter.runScripts(*this, ctx);
}