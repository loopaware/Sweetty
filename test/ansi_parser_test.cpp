#include "gtest/gtest.h"
#include "ansi_parser.h"
#include "terminal_buffer.h"

TEST(AnsiParserTest, ParseSimpleText) {
    TerminalBuffer buffer(80, 24);
    AnsiParser parser(&buffer);
    parser.parse("Hello, world!");
    const auto& text = buffer.getBuffer();
    ASSERT_EQ(text[0][0].value, 'H');
    ASSERT_EQ(text[0][1].value, 'e');
    ASSERT_EQ(text[0][12].value, '!');
}

TEST(AnsiParserTest, ParseCursorMovement) {
    TerminalBuffer buffer(80, 24);
    AnsiParser parser(&buffer);
    parser.parse("\033[5;10H");
    ASSERT_EQ(buffer.getCursorRow(), 4);
    ASSERT_EQ(buffer.getCursorCol(), 9);
}

TEST(AnsiParserTest, ParseInvalidEscapeCode) {
    TerminalBuffer buffer(80, 24);
    AnsiParser parser(&buffer);
    parser.parse("\033[1;2;3z");
    // Nothing should happen, and it should not crash
}
