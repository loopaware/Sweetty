#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>
#include <QSurfaceFormat>
#include <QTimer>
#include <iostream>

class TerminalWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_0
{
public:
    TerminalWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent)
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