#ifndef ANSI_PARSER_H
#define ANSI_PARSER_H

#include <string>
#include "terminal_buffer.h"

class AnsiParser {
public:
    AnsiParser(TerminalBuffer* buffer);
    void parse(const std::string& text);

private:
    void handleEscapeCode(const std::string& code);
    TerminalBuffer* terminalBuffer_;
    std::string escapeCodeBuffer_;
    bool inEscapeSequence_ = false;
};

#endif // ANSI_PARSER_H
