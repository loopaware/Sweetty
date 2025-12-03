#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>
#include <QSurfaceFormat>
#include <QTimer>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp> // For matrix operations
#include <glm/gtc/matrix_transform.hpp> // For orthographic projection

#include <ft2build.h>
#include FT_FREETYPE_H
#include "terminal_buffer.h" // Include TerminalBuffer header
#include "pty_process.h" // Include PtyProcess header
#include <QKeyEvent> // Include QKeyEvent

struct Character {
    GLuint     textureID;  // ID handle of the glyph texture
    QSize      size;       // Size of glyph
    QPoint     bearing;    // Offset from baseline to left/top of glyph
    GLuint     advance;    // Offset to advance to next glyph
};

class TerminalWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_0
{
public:
    TerminalWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent), ft_library(nullptr), ft_face(nullptr), fontAtlasTexture(0), shaderProgram(0), VAO(0), VBO(0), terminalBuffer(nullptr), ptyProcess(nullptr)
    {
        // Request OpenGL ES 3.0 context
        QSurfaceFormat format;
        format.setRenderableType(QSurfaceFormat::OpenGLES);
        format.setVersion(3, 0);
        format.setProfile(QSurfaceFormat::NoProfile); // ES does not have compatibility profile
        setFormat(format);

        // Schedule an update for every frame
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&TerminalWidget::update));
        timer->start(16); // Approximately 60 FPS
    }

    ~TerminalWidget()
    {
        if (fontAtlasTexture) {
            glDeleteTextures(1, &fontAtlasTexture);
        }
        if (ft_face) {
            FT_Done_Face(ft_face);
        }
        if (ft_library) {
            FT_Done_FreeType(ft_library);
        }
        if (shaderProgram) {
            glDeleteProgram(shaderProgram);
        }
        if (VAO) {
            glDeleteVertexArrays(1, &VAO);
        }
        if (VBO) {
            glDeleteBuffers(1, &VBO);
        }
        if (terminalBuffer) {
            delete terminalBuffer;
        }
        if (ptyProcess) {
            delete ptyProcess;
        }
    }

protected:
    GLuint compileShader(GLenum type, const char* path)
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

    GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath)
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


    void initializeGL() override
    {
        initializeOpenGLFunctions(); // Initialize OpenGL functions for ES 3.0

        if (!isValid()) {
            std::cerr << "Failed to create valid OpenGL context!" << std::endl;
            // Handle error, maybe quit application
            return;
        }

        std::cout << "OpenGL Vendor: " << (const char*)glGetString(GL_VENDOR) << std::endl;
        std::cout << "OpenGL Renderer: " << (const char*)glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL Version: " << (const char*)glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL Version: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set background color

        // --- FreeType Initialization ---
        FT_Error error = FT_Init_FreeType(&ft_library);
        if (error) {
            std::cerr << "FreeType: Could not init FreeType Library" << std::endl;
            return;
        }

        // TODO: Make font path configurable
        const char* font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
        error = FT_New_Face(ft_library, font_path, 0, &ft_face);
        if (error == FT_Err_Unknown_File_Format) {
            std::cerr << "FreeType: The font file could be opened and read, but it appears that its font format is unsupported." << std::endl;
            return;
        } else if (error) {
            std::cerr << "FreeType: Could not open/read font file: " << font_path << std::endl;
            return;
        }

        // Set font size. 64 pixels for better visibility.
        // TODO: Make font size configurable and adapt to DPI
        error = FT_Set_Pixel_Sizes(ft_face, 0, 64);
        if (error) {
            std::cerr << "FreeType: Could not set font pixel size." << std::endl;
            return;
        }
        std::cout << "FreeType initialized and font loaded: " << font_path << std::endl;

        // --- Glyph Texture Atlas Generation ---
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        glGenTextures(1, &fontAtlasTexture);
        glBindTexture(GL_TEXTURE_2D, fontAtlasTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Estimate atlas size (rough estimate, can be improved with packing algorithm)
        // A common size for text atlases might be 1024x1024 or 2048x2048.
        // For now, let's just make it big enough for typical ASCII glyphs.
        // Assuming ~50px height per glyph and ~100 glyphs, 1024x1024 might be enough.
        int atlasWidth = 1024;
        int atlasHeight = 1024;
        // Allocate texture on GPU
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_R8, // Single channel for grayscale glyphs (alpha only)
            atlasWidth,
            atlasHeight,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            nullptr
        );

        int xOffset = 0;
        int yOffset = 0;
        int rowHeight = 0;

        for (unsigned char c = 32; c < 128; c++) // ASCII characters 32-127
        {
            if (FT_Load_Char(ft_face, c, FT_LOAD_RENDER)) 
            {
                std::cerr << "FreeType: Failed to load Glyph for character: " << c << std::endl;
                continue;
            }

            if (xOffset + ft_face->glyph->bitmap.width > atlasWidth) {
                xOffset = 0;
                yOffset += rowHeight;
                rowHeight = 0;
            }

            if (yOffset + ft_face->glyph->bitmap.rows > atlasHeight) {
                std::cerr << "Glyph Atlas: Not enough space in texture atlas for character: " << c << std::endl;
                // This is a critical error, needs handling or a larger atlas
                break;
            }

            glTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                xOffset,
                yOffset,
                ft_face->glyph->bitmap.width,
                ft_face->glyph->bitmap.rows,
                GL_RED,
                GL_UNSIGNED_BYTE,
                ft_face->glyph->bitmap.buffer
            );

            Character character = {
                fontAtlasTexture,
                QSize(ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows),
                QPoint(ft_face->glyph->bitmap_left, ft_face->glyph->bitmap_top),
                static_cast<GLuint>(ft_face->glyph->advance.x >> 6)
            };
            Characters.insert(std::pair<char, Character>(c, character));

            xOffset += ft_face->glyph->bitmap.width;
            rowHeight = std::max(rowHeight, static_cast<int>(ft_face->glyph->bitmap.rows));
        }

        std::cout << "Glyph Texture Atlas generated for ASCII characters." << std::endl;

        // --- Shader Program Setup ---
        shaderProgram = createShaderProgram("shaders/text.vert", "shaders/text.frag");
        if (shaderProgram == 0) {
            std::cerr << "Failed to create shader program!" << std::endl;
            return;
        }

        glUseProgram(shaderProgram);
        glUniform1i(glGetUniformLocation(shaderProgram, "text"), 0); // Set texture sampler to unit 0

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // --- Terminal Buffer Setup ---
        // Calculate terminal dimensions based on font metrics and widget size
        // For simplicity, let's assume average character width and height
        int charWidth = (ft_face->glyph->advance.x >> 6); // Average width for monospaced font
        int charHeight = ft_face->size->metrics.height >> 6; // Average height including ascender/descender
        if (charWidth == 0) charWidth = 1; // Avoid division by zero
        if (charHeight == 0) charHeight = 1;

        int cols = width() / charWidth;
        int rows = height() / charHeight;

        terminalBuffer = new TerminalBuffer(cols, rows);
        terminalBuffer->write("Hello, Sweetty GL!");
        terminalBuffer->newLine();
        terminalBuffer->write("This is a terminal emulator.");
        terminalBuffer->newLine();
        terminalBuffer->write("It's GPU accelerated!");
        terminalBuffer->newLine();
        terminalBuffer->write("With FreeType and HarfBuzz.");
        
        // --- PtyProcess Setup ---
        ptyProcess = new PtyProcess(terminalBuffer);
        if (!ptyProcess->startShell("/bin/bash")) {
            std::cerr << "Failed to start shell!" << std::endl;
            // Handle error, maybe quit application
        }
    }

    void resizeGL(int w, int h) override
    {
        glViewport(0, 0, w, h);

        if (terminalBuffer) {
            // Recalculate terminal dimensions based on new widget size
            int charWidth = (ft_face->glyph->advance.x >> 6);
            int charHeight = ft_face->size->metrics.height >> 6;
            if (charWidth == 0) charWidth = 1;
            if (charHeight == 0) charHeight = 1;

            int newCols = w / charWidth;
            int newRows = h / charHeight;

            terminalBuffer->resize(newCols, newRows);
            if (ptyProcess) {
                ptyProcess->resize(newCols, newRows);
            }
        }
    }

    void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUseProgram(shaderProgram);
        glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), 1.0f, 1.0f, 1.0f); // White color

        // Setup orthographic projection matrix
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width()), 0.0f, static_cast<float>(height()));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Render characters from terminal buffer
        // Position from top-left, rows are from top to bottom
        float current_y = static_cast<float>(height()); 
        float charHeight = ft_face->size->metrics.height >> 6;

        for (const auto& row : terminalBuffer->getBuffer()) {
            float current_x = 0.0f;
            current_y -= charHeight; // Move down by character height for each row

            for (const auto& termChar : row) {
                char c = termChar.value;
                if (Characters.count(c)) {
                    Character ch = Characters[c];

                    GLfloat xpos = current_x + ch.bearing.x();
                    GLfloat ypos = current_y + ch.bearing.y(); 

                    GLfloat w = ch.size.width();
                    GLfloat h = ch.size.height();

                    // Update VBO for each character
                    GLfloat vertices[6][4] = {
                        { xpos,     ypos + h,   0.0f, 0.0f },
                        { xpos,     ypos,       0.0f, 1.0f },
                        { xpos + w, ypos,       1.0f, 1.0f },

                        { xpos,     ypos + h,   0.0f, 0.0f },
                        { xpos + w, ypos,       1.0f, 1.0f },
                        { xpos + w, ypos + h,   1.0f, 0.0f }
                    };

                    // Render glyph texture over quad
                    glBindTexture(GL_TEXTURE_2D, ch.textureID);
                    // Update content of VBO memory
                    glBindBuffer(GL_ARRAY_BUFFER, VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    // Render quad
                    glDrawArrays(GL_TRIANGLES, 0, 6);

                    // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                    current_x += (ch.advance);
                } else {
                    // If character not in atlas, just advance by average width
                    current_x += (ft_face->glyph->advance.x >> 6);
                }
            }
        }

        glDisable(GL_BLEND);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }

protected: // Add keyPressEvent here
    void keyPressEvent(QKeyEvent *event) override
    {
        if (ptyProcess) {
            // Handle special keys
            if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
                ptyProcess->write("\n");
            } else if (event->key() == Qt::Key_Backspace) {
                ptyProcess->write("\b");
            } else if (event->key() == Qt::Key_Tab) {
                ptyProcess->write("\t");
            } else if (event->text().length() > 0) { // Check if it's a printable character
                ptyProcess->write(event->text().toStdString());
            }
            update(); // Request a repaint
        }
        QOpenGLWidget::keyPressEvent(event); // Call base class implementation
    }
private:
    FT_Library ft_library;
    FT_Face ft_face;
    GLuint fontAtlasTexture;
    std::map<char, Character> Characters;

    GLuint shaderProgram;
    GLuint VAO, VBO;
    TerminalBuffer* terminalBuffer; // Member for TerminalBuffer
}; // Missing closing brace for TerminalWidget class


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Request OpenGL ES 3.0 context for the whole application
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGLES);
    format.setVersion(3, 0);
    format.setProfile(QSurfaceFormat::NoProfile);
    QSurfaceFormat::setDefaultFormat(format);

    TerminalWidget w;
    w.setWindowTitle("Sweetty GL Terminal - Qt");
    w.resize(800, 600);
    w.show();

    return a.exec();
}
