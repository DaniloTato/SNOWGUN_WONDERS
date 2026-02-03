#include "SnowErr.hpp"
#include <sstream>

SnowErr::SnowErr(Diagnostic d) : diag(std::move(d)) {}

const char *SnowErr::what() const noexcept { return diag.message.c_str(); }

std::string SnowErr::format(const std::string &source) const {
  std::ostringstream out;

  out << "\nerror[" << phaseToString(diag.phase) << "]: " << diag.message << "\n";

  out << "  --> line " << diag.span.line << ", column " << diag.span.column << "\n";

  printSourceLine(out, source, diag.span);

  for (const auto &note : diag.notes) {
    out << " note: " << note << "\n";
  }

  return out.str();
}

void SnowErr::printSourceLine(std::ostream &out, const std::string &source,
                              const SourceSpan &span) {
  size_t lineStart = 0;
  size_t currentLine = 1;

  for (size_t i = 0; i < source.size(); ++i) {
    if (currentLine == span.line)
      break;
    if (source[i] == '\n') {
      lineStart = i + 1;
      ++currentLine;
    }
  }

  size_t lineEnd = source.find('\n', lineStart);
  if (lineEnd == std::string::npos)
    lineEnd = source.size();

  std::string lineText = source.substr(lineStart, lineEnd - lineStart);

  out << "   |\n";
  out << " " << span.line << " | " << lineText << "\n";
  out << "   | ";

  for (size_t i = 1; i < span.column; ++i)
    out << ' ';

  out << "^\n";
}

[[noreturn]] void throwError(SnowErr::Phase phase, const std::string &message,
                             const SourceSpan &span) {
  throw SnowErr({.phase = phase, .message = message, .span = span, .notes = {}});
}

[[noreturn]] void throwError(SnowErr::Phase phase, const std::string &message,
                             const SourceSpan &span, const std::vector<std::string> &notes) {

  throw SnowErr({.phase = phase, .message = message, .span = span, .notes = notes});
}

[[noreturn]] void throwError(SnowErr::Phase phase, const std::string &message,
                             const SourceSpan &span, const std::string &note) {
  throw SnowErr({.phase = phase, .message = message, .span = span, .notes = {note}});
}

const char *phaseToString(SnowErr::Phase p) {
  switch (p) {
  case SnowErr::Phase::Tokenizer:
    return "TOKENIZER";
  case SnowErr::Phase::Parser:
    return "PARSER";
  case SnowErr::Phase::Resolver:
    return "RESOLVER";
  case SnowErr::Phase::Evaluator:
    return "EVALUATOR";
  case SnowErr::Phase::Memory:
    return "MEMORY";
  }
  return "unknown";
}