#pragma once
#include "SnowIO.hpp"

class Terminal;

namespace Snowlang {

class SnowTermIO final : public SnowIO {
public:
  explicit SnowTermIO(Terminal &terminal);

  void write(const std::string &text) override;
  void writeLn(const std::string &text) override;

  bool hasLine() override;
  std::string readLine() override;

  bool writeFile(const std::string &path, const std::string &contents, bool append) override;

  void pushLine(std::string line);

private:
  Terminal &terminal;
  std::queue<std::string> inputQueue;
};

} // namespace Snowlang