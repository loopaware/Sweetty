#include "terminal_buffer.h"
#include <iostream>

TerminalBuffer::TerminalBuffer(int width, int height)
    : width_(width),
      height_(height),
      cursorRow_(0),
      cursorCol_(0)
{
    buffer_.resize(height_, std::vector<TerminalChar>(width_, {' '}));
}

void TerminalBuffer::write(char c) {
    if (cursorCol_ >= width_) {
        newLine();
    }
    buffer_[cursorRow_][cursorCol_].value = c;
    cursorCol_++;
}

void TerminalBuffer::write(const std::string& text) {
    for (char c : text) {
        write(c);
    }
}

void TerminalBuffer::newLine() {
    cursorCol_ = 0;
    cursorRow_++;
    if (cursorRow_ >= height_) {
        scrollUp();
        cursorRow_ = height_ - 1; // Keep cursor on the last line after scroll
    }
}

void TerminalBuffer::moveCursor(int row, int col) {
    if (row >= 0 && row < height_) {
        cursorRow_ = row;
    } else {
        std::cerr << "Warning: Attempted to move cursor to invalid row: " << row << std::endl;
    }

    if (col >= 0 && col < width_) {
        cursorCol_ = col;
    } else {
        std::cerr << "Warning: Attempted to move cursor to invalid column: " << col << std::endl;
    }
}

void TerminalBuffer::scrollUp() {
    // Shift all rows up by one
    for (int i = 0; i < height_ - 1; ++i) {
        buffer_[i] = buffer_[i+1];
    }
    // Clear the last row
    buffer_[height_ - 1] = std::vector<TerminalChar>(width_, {' '});
}

void TerminalBuffer::resize(int newWidth, int newHeight) {
    if (newWidth <= 0 || newHeight <= 0) {
        std::cerr << "Error: Attempted to resize TerminalBuffer to invalid dimensions: " << newWidth << "x" << newHeight << std::endl;
        return;
    }

    std::vector<std::vector<TerminalChar>> newBuffer(newHeight, std::vector<TerminalChar>(newWidth, {' '}));

    // Copy existing content to the new buffer
    int copyRows = std::min(height_, newHeight);
    int copyCols = std::min(width_, newWidth);

    for (int r = 0; r < copyRows; ++r) {
        for (int c = 0; c < copyCols; ++c) {
            newBuffer[r][c] = buffer_[r][c];
        }
    }

    buffer_ = std::move(newBuffer);
    width_ = newWidth;
    height_ = newHeight;

    // Adjust cursor position to be within new bounds
    if (cursorRow_ >= newHeight) {
        cursorRow_ = newHeight - 1;
    }
    if (cursorCol_ >= newWidth) {
        cursorCol_ = newWidth - 1;
    }
}

