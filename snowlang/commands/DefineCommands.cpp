#include "Commands.hpp"
#include "SnowlangInstance.hpp"

namespace Snowlang::Commands {

void DefineCommands(Snowlang::SnowlangInstance &snowlang) {
  size_t echoId = snowlang.resolver.getNewCommandId("echo");
  snowlang.memory.setCommandById(echoId,
                                 Snowlang::CommandSignature(Snowlang::Commands::echoCommand));
  size_t listId = snowlang.resolver.getNewCommandId("list");
  snowlang.memory.setCommandById(listId,
                                 Snowlang::CommandSignature(Snowlang::Commands::listCommand));
  size_t runId = snowlang.resolver.getNewCommandId("run");
  snowlang.memory.setCommandById(runId, Snowlang::CommandSignature(Snowlang::Commands::runCommand));
  size_t printId = snowlang.resolver.getNewCommandId("print");
  snowlang.memory.setCommandById(printId,
                                 Snowlang::CommandSignature(Snowlang::Commands::printCommand));
  size_t writeId = snowlang.resolver.getNewCommandId("write");
  snowlang.memory.setCommandById(writeId,
                                 Snowlang::CommandSignature(Snowlang::Commands::writeCommand));
  size_t watchId = snowlang.resolver.getNewCommandId("watch");
  snowlang.memory.setCommandById(watchId,
                                 Snowlang::CommandSignature(Snowlang::Commands::watchCommand));
  size_t taskId = snowlang.resolver.getNewCommandId("task");
  snowlang.memory.setCommandById(taskId,
                                 Snowlang::CommandSignature(Snowlang::Commands::taskCommand));
  size_t gameGetId = snowlang.resolver.getNewCommandId("game_get");
  snowlang.memory.setCommandById(gameGetId,
                                 Snowlang::CommandSignature(Snowlang::Commands::gameGetCommand));
}

} // namespace Snowlang::Commands