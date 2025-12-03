#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>
#include <QSurfaceFormat>
#include <QTimer>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

class TerminalWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_0
{
public:
    TerminalWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent), ft_library(nullptr), ft_face(nullptr)
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