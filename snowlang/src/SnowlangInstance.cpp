#include "SnowlangInstance.hpp"

#include "Debug.hpp"
#include "Parser.hpp"
#include "Resolver.hpp"
#include "SnowErr.hpp"
#include "Tokenizer.hpp"

#include <fstream>
#include <sstream>

SnowlangInstance::SnowlangInstance(SnowIO &ioInterface)
    : memory(this), resolver(this), evaluator(this), io(ioInterface), debug(ioInterface) {
  Commands::DefineCommands(*this);
}

void SnowlangInstance::run(const std::string &source) {
  if (source.empty()) {
    return;
  }

  cacheSource(source);
  try {
    Tokenizer tokenizer(source);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens);

    std::vector<StmtPtr> ast;
    if (Debug::enabled) {
      ast = parser.parseAndDebugAST(io);
    } else {
      ast = parser.parseProgram();
    }

    for (const auto &stmt : ast) {
      RStmtPtr resolved = resolver.resolveStmt(stmt);
      evaluator.execStmt(resolved);
    }
  } catch (const SnowErr &err) {
    io.writeLn(err.format(cachedSource));
  }
}

std::string SnowlangInstance::readFile(const std::string &path) const {
  std::ifstream file(path, std::ios::in);
  if (!file.is_open()) {
    throw std::runtime_error("[IO] Could not open file: " + path);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void SnowlangInstance::cacheSource(const std::string &source) { cachedSource = source; }

SnowlangInstance::Module SnowlangInstance::loadModule(const std::string &path) {
  std::string source = readFile(path);
  cacheSource(source);

  // im sure there must be a more elegant solution to this
  // I'll improve it later.
  auto previousGlobals = resolver.getGlobalSymbols();

  Tokenizer tokenizer(source);
  Parser parser(tokenizer.tokenize());
  auto ast = parser.parseProgram();

  std::vector<RStmtPtr> resolved;
  resolved.reserve(ast.size());
  for (auto &stmt : ast) {
    resolved.push_back(resolver.resolveStmt(stmt));
  }

  for (auto &stmt : resolved) {
    evaluator.execStmt(stmt);
  }

  Module m;
  for (auto &[name, slot] : resolver.getGlobalSymbols()) {
    if (!previousGlobals.count(name)) {
      m.exportSlots[name] = slot;
    }
  }

  return m;
}

void SnowlangInstance::update(double deltaTime) {
  for (auto &task : scheduledTasks) {

    if (task.isPaused()) {
      continue;
    }

    task.accumulator += deltaTime;

    while (task.accumulator >= task.interval) {
      task.accumulator -= task.interval;
      evaluator.runLambda(task.lambda);
    }
  }
}

void SnowlangInstance::scheduleTask(ScheduledTask &&task) {
  scheduledTasks.push_back(std::move(task));
}

bool SnowlangInstance::killTask(size_t id) {
  auto it = std::ranges::remove_if(scheduledTasks, [&](const ScheduledTask &t) {
              return t.id == id;
            }).begin();

  bool removed = it != scheduledTasks.end();
  scheduledTasks.erase(it, scheduledTasks.end());
  return removed;
}

void SnowlangInstance::killAllTasks() { scheduledTasks.clear(); }

bool SnowlangInstance::togglePauseTask(size_t id) {
  auto it = std::ranges::remove_if(scheduledTasks, [&](const ScheduledTask &t) {
              return t.id == id;
            }).begin();

  if (it == scheduledTasks.end()) {
    return false;
  }

  it->togglePause();
  return true;
}

size_t SnowlangInstance::taskCount() const { return scheduledTasks.size(); }

size_t SnowlangInstance::getNewTaskId() { return nextTaskId; }

const std::vector<ScheduledTask> &SnowlangInstance::getTasks() const { return scheduledTasks; }