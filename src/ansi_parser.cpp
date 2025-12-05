#include "ansi_parser.h"
#include <iostream>

AnsiParser::AnsiParser(TerminalBuffer* buffer) : terminalBuffer_(buffer) {}

void AnsiParser::parse(const std::string& text) {
    for (char c : text) {
        if (inEscapeSequence_) {
            escapeCodeBuffer_ += c;
            if (isalpha(c)) {
                handleEscapeCode(escapeCodeBuffer_);
                inEscapeSequence_ = false;
                escapeCodeBuffer_.clear();
            }
        } else if (c == '\033') { // Escape character
            inEscapeSequence_ = true;
            escapeCodeBuffer_ += c;
        } else {
            terminalBuffer_->write(std::string(1, c));
        }
    }
}

void AnsiParser::handleEscapeCode(const std::string& code) {
    // Basic CSI handling
    if (code.rfind("\033[", 0) == 0) {
        std::string csi_sequence = code.substr(2);
        char command = csi_sequence.back();
        std::string params_str = csi_sequence.substr(0, csi_sequence.length() - 1);
        
        // Split parameters
        std::vector<int> params;
        std::string current_param;
        for (char p : params_str) {
            if (isdigit(p)) {
                current_param += p;
            } else if (p == ';') {
                params.push_back(current_param.empty() ? 0 : std::stoi(current_param));
                current_param.clear();
            }
        }
        if (!current_param.empty()) {
            params.push_back(std::stoi(current_param));
        }


        switch (command) {
            case 'A': // CUU - Cursor Up
                if (params.empty()) {
                    terminalBuffer_->moveCursor(terminalBuffer_->getCursorRow() - 1, terminalBuffer_->getCursorCol());
                } else {
                    terminalBuffer_->moveCursor(terminalBuffer_->getCursorRow() - params[0], terminalBuffer_->getCursorCol());
                }
                break;
            case 'B': // CUD - Cursor Down
                if (params.empty()) {
                    terminalBuffer_->moveCursor(terminalBuffer_->getCursorRow() + 1, terminalBuffer_->getCursorCol());
                } else {
                    terminalBuffer_->moveCursor(terminalBuffer_->getCursorRow() + params[0], terminalBuffer_->getCursorCol());
                }
                break;
            case 'C': // CUF - Cursor Forward
                if (params.empty()) {
                    terminalBuffer_->moveCursor(terminalBuffer_->getCursorRow(), terminalBuffer_->getCursorCol() + 1);
                } else {
                    terminalBuffer_->moveCursor(terminalBuffer_->getCursorRow(), terminalBuffer_->getCursorCol() + params[0]);
                }
                break;
            case 'D': // CUB - Cursor Back
                if (params.empty()) {
                    terminalBuffer_->moveCursor(terminalBuffer_->getCursorRow(), terminalBuffer_->getCursorCol() - 1);
                } else {
                    terminalBuffer_->moveCursor(terminalBuffer_->getCursorRow(), terminalBuffer_->getCursorCol() - params[0]);
                }
                break;
            case 'H': // CUP - Cursor Position
                 if (params.size() >= 2) {
                    terminalBuffer_->moveCursor(params[0] - 1, params[1] - 1);
                } else {
                    terminalBuffer_->moveCursor(0, 0);
                }
                break;
            // TODO: Add more ANSI escape code handling here
            default:
                std::cerr << "Unsupported ANSI escape code: " << code << std::endl;
                break;
        }
    }
}
