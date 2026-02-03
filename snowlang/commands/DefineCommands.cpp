#include "Commands.hpp"
#include "SnowlangInstance.hpp"

namespace Commands {

void DefineCommands(SnowlangInstance &snowlang) {
  size_t echoId = snowlang.resolver.getNewCommandId("echo");
  snowlang.memory.setCommandById(echoId, CommandSignature(Commands::echoCommand));
  size_t listId = snowlang.resolver.getNewCommandId("list");
  snowlang.memory.setCommandById(listId, CommandSignature(Commands::listCommand));
  size_t runId = snowlang.resolver.getNewCommandId("run");
  snowlang.memory.setCommandById(runId, CommandSignature(Commands::runCommand));
  size_t printId = snowlang.resolver.getNewCommandId("print");
  snowlang.memory.setCommandById(printId, CommandSignature(Commands::printCommand));
  size_t writeId = snowlang.resolver.getNewCommandId("write");
  snowlang.memory.setCommandById(writeId, CommandSignature(Commands::writeCommand));
  size_t watchId = snowlang.resolver.getNewCommandId("watch");
  snowlang.memory.setCommandById(watchId, CommandSignature(Commands::watchCommand));
  size_t taskId = snowlang.resolver.getNewCommandId("task");
  snowlang.memory.setCommandById(taskId, CommandSignature(Commands::taskCommand));
}

} // namespace Commands