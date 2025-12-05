#include "terminal_widget.h"
#include "font_manager.h"
#include "terminal_buffer.h"
#include "pty_process.h"
#include "renderer.h"
#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>
#include <QSurfaceFormat>
#include <QTimer>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QKeyEvent>
#include <QSocketNotifier>
#include <QRectF>
#include <QFileDialog>

TerminalWidget::TerminalWidget(QWidget *parent) : QOpenGLWidget(parent), fontManager(std::make_unique<FontManager>()), renderer(std::make_unique<Renderer>())
{
    // Schedule an update for every frame
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&TerminalWidget::update));
    timer->start(16); // Approximately 60 FPS
}

TerminalWidget::~TerminalWidget()
{
}

void TerminalWidget::openFontDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Font", "", "Font Files (*.ttf *.otf)");
    if (!fileName.isEmpty()) {
        setFont(fileName.toStdString());
    }
}

void TerminalWidget::setFont(const std::string& fontPath)
{
    makeCurrent();
    fontManager->loadFont(fontPath, 16);
    doneCurrent();
    update();
}

void TerminalWidget::initializeGL()
{
    initializeOpenGLFunctions();

    if (!isValid()) {
        std::cerr << "Failed to create valid OpenGL context!" << std::endl;
        return;
    }

    renderer->initialize(width(), height());
    fontManager->loadFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 16);

    int charWidth = 0;
    int charHeight = 0;
    try {
        const auto& ch = fontManager->getCharacter('A');
        charWidth = ch.Advance >> 6;
        charHeight = ch.Size.y;
    } catch (const std::out_of_range& e) {
        std::cerr << "Could not get font metrics." << std::endl;
    }
    if (charWidth == 0) charWidth = 1;
    if (charHeight == 0) charHeight = 16;

    int cols = width() / charWidth;
    int rows = height() / charHeight;

    terminalBuffer = std::make_unique<TerminalBuffer>(cols, rows);
    terminalBuffer->write("Hello, Sweetty GL!");
    terminalBuffer->newLine();
    terminalBuffer->write("This is a terminal emulator.");
    terminalBuffer->newLine();
    terminalBuffer->write("It's GPU accelerated!");
    terminalBuffer->newLine();
    terminalBuffer->write("With FreeType and HarfBuzz.");
}

void TerminalWidget::resizeGL(int w, int h)
{
    renderer->resize(w, h);

    if (terminalBuffer) {
        int charWidth = 0;
        int charHeight = 0;
        try {
            const auto& ch = fontManager->getCharacter('A');
            charWidth = ch.Advance >> 6;
            charHeight = ch.Size.y;
        } catch (const std::out_of_range& e) {
            std::cerr << "Could not get font metrics." << std::endl;
        }
        if (charWidth == 0) charWidth = 1;
        if (charHeight == 0) charHeight = 16;

        int newCols = w / charWidth;
        int newRows = h / charHeight;

        terminalBuffer->resize(newCols, newRows);
    }
}

void TerminalWidget::paintGL()
{
    renderer->paint(fontManager.get(), terminalBuffer.get());
}

void TerminalWidget::keyPressEvent(QKeyEvent *event)
{
    // Do nothing for now
    QOpenGLWidget::keyPressEvent(event);
}
