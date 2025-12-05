#include "renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer() : shaderProgram(0), VAO(0), VBO(0), width(0), height(0) {}

Renderer::~Renderer() {
    if (shaderProgram) {
        glDeleteProgram(shaderProgram);
    }
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
    }
}

void Renderer::initialize(float width, float height) {
    this->width = width;
    this->height = height;
    initializeOpenGLFunctions();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    shaderProgram = createShaderProgram("shaders/text.vert", "shaders/text.frag");
    if (shaderProgram == 0) {
        std::cerr << "Failed to create shader program!" << std::endl;
        return;
    }

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "text"), 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::resize(float width, float height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
}

void Renderer::paint(FontManager* fontManager, TerminalBuffer* terminalBuffer) {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), 1.0f, 1.0f, 1.0f);

    glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, fontManager->getAtlasTextureId());

    int charHeight = 0;
    try {
        const auto& ch = fontManager->getCharacter('A');
        charHeight = ch.Size.y;
    } catch (const std::out_of_range& e) {
        std::cerr << "Could not get font metrics." << std::endl;
    }
    if (charHeight == 0) charHeight = 16;
    float current_y = height - charHeight;

    std::vector<GLfloat> vertices;
    vertices.reserve(terminalBuffer->getWidth() * terminalBuffer->getHeight() * 6 * 4);

    for (const auto& row : terminalBuffer->getBuffer()) {
        float current_x = 0.0f;

        for (const auto& termChar : row) {
            char32_t c = termChar.value;
            if (c == ' ') {
                current_x += (fontManager->getCharacter('A').Advance >> 6);
                continue;
            }
            try {
                const Character& ch = fontManager->getCharacter(c);

                GLfloat xpos = current_x + ch.Bearing.x;
                GLfloat ypos = current_y - (ch.Size.y - ch.Bearing.y);

                GLfloat w = ch.Size.x;
                GLfloat h = ch.Size.y;

                GLfloat character_vertices[] = {
                    xpos,     ypos + h,   0.0f, 0.0f,
                    xpos,     ypos,       0.0f, 1.0f,
                    xpos + w, ypos,       1.0f, 1.0f,

                    xpos,     ypos + h,   0.0f, 0.0f,
                    xpos + w, ypos,       1.0f, 1.0f,
                    xpos + w, ypos + h,   1.0f, 0.0f
                };
                vertices.insert(vertices.end(), std::begin(character_vertices), std::end(character_vertices));

                current_x += (ch.Advance >> 6);
            } catch (const std::out_of_range& e) {
                int advance = 0;
                try {
                    const auto& ch = fontManager->getCharacter('A');
                    advance = ch.Advance >> 6;
                } catch (const std::out_of_range& e) {
                    advance = 8;
                }
                current_x += advance;
            }
        }
        current_y -= charHeight;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 4);

    glDisable(GL_BLEND);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

GLuint Renderer::compileShader(GLenum type, const char* path)
{
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path << " " << e.what() << std::endl;
        return 0;
    }
    const char* sCode = shaderCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &sCode, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED of " << path << "\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

GLuint Renderer::createShaderProgram(const char* vertexPath, const char* fragmentPath)
{
    GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexPath);
    GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

    if (vertex == 0 || fragment == 0) {
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}
