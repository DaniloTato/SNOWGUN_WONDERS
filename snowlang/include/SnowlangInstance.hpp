#pragma once
#include "Debug.hpp"
#include "Evaluator.hpp"
#include "Memory.hpp"
#include "Resolver.hpp"
#include "SnowIO.hpp"

#include "ScheduledTask.hpp"
#include <string>

class SnowlangInstance {
private:
  struct Module {
    std::unordered_map<std::string, size_t> exportSlots;
  };

private:
  std::string cachedSource;
  void cacheSource(const std::string &source);

  std::vector<ScheduledTask> scheduledTasks;
  size_t nextTaskId = 0;

public:
  SnowlangInstance(SnowIO &ioInterface);
  void run(const std::string &source);
  [[nodiscard]] std::string readFile(const std::string &path) const;
  Module loadModule(const std::string &path);
  void update(double deltaTime);
  [[nodiscard]] const std::vector<ScheduledTask> &getTasks() const;

  bool togglePauseTask(size_t id);

  void scheduleTask(ScheduledTask &&task);
  bool killTask(size_t id);
  void killAllTasks();
  [[nodiscard]] size_t taskCount() const;

  size_t getNewTaskId();

public:
  Memory memory;
  Resolver resolver;
  Evaluator evaluator;
  SnowIO &io;
  Debug debug;
};