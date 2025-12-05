#ifndef TERMINAL_BUFFER_H
#define TERMINAL_BUFFER_H

#include <vector>
#include <string>

// Forward declaration for Character if needed, or define it here if simple
// For now, let's just store chars directly. We can enhance this later with styling info.
struct TerminalChar {
    char32_t value;
    // Add attributes like foreground/background color, bold, italic, etc. later
    // QColor foreground;
    // QColor background;
    // bool bold;
    // bool italic;
};

class TerminalBuffer {
public:
    TerminalBuffer(int width, int height);

    void write(char32_t c);
    void write(const std::string& text);
    void newLine();
    void moveCursor(int row, int col);

    // Getters
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    int getCursorRow() const { return cursorRow_; }
    int getCursorCol() const { return cursorCol_; }
    const std::vector<std::vector<TerminalChar>>& getBuffer() const { return buffer_; }

    void resize(int width, int height);

private:
    void scrollUp();

    int width_;
    int height_;
    int cursorRow_;
    int cursorCol_;
    std::vector<std::vector<TerminalChar>> buffer_;
};

#endif // TERMINAL_BUFFER_H
