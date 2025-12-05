#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions_3_0>
#include <memory>
#include "font_manager.h"
#include "terminal_buffer.h"

class Renderer : protected QOpenGLFunctions_3_0 {
public:
    Renderer();
    ~Renderer();

    void initialize(float width, float height);
    void resize(float width, float height);
    void paint(FontManager* fontManager, TerminalBuffer* terminalBuffer);

private:
    GLuint compileShader(GLenum type, const char* path);
    GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);

    GLuint shaderProgram;
    GLuint VAO, VBO;
    float width, height;
};

#endif // RENDERER_H
