#ifndef TERMINAL_WIDGET_H
#define TERMINAL_WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>
#include <memory>

class FontManager;
class TerminalBuffer;
class Renderer;

class TerminalWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_0
{
    Q_OBJECT

public:
    TerminalWidget(QWidget *parent = nullptr);
    ~TerminalWidget();

public slots:
    void openFontDialog();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setFont(const std::string& fontPath);

    std::unique_ptr<FontManager> fontManager;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<TerminalBuffer> terminalBuffer;
};

#endif // TERMINAL_WIDGET_H
