#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>
#include <QSurfaceFormat>
#include <QTimer>
#include <iostream>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    GLuint     textureID;  // ID handle of the glyph texture
    QSize      size;       // Size of glyph
    QPoint     bearing;    // Offset from baseline to left/top of glyph
    GLuint     advance;    // Offset to advance to next glyph
};

class TerminalWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_0
{
public:
    TerminalWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent), ft_library(nullptr), ft_face(nullptr), fontAtlasTexture(0)
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
    }

protected:
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

        // Set font size to 48 pixels, 0 for horizontal resolution uses device resolution
        error = FT_Set_Pixel_Sizes(ft_face, 0, 48);
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
    }

    void resizeGL(int w, int h) override
    {
        glViewport(0, 0, w, h);
    }

    void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
        // Rendering commands will go here
    }

private:
    FT_Library ft_library;
    FT_Face ft_face;
    GLuint fontAtlasTexture;
    std::map<char, Character> Characters;
};

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