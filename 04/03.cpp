/**
 * Two VBOs: one for position and another for color.
 */
#include <tinygl/tinygl.h>
#include <tinygl/main.h>
#include <array>
#include <iostream>
#include <sstream>

const char* windowTitle = NAME;

constexpr auto vert_pos = std::array {
     0.0f,  0.5f, 0.0f,	// Top
     0.5f, -0.5f, 0.0f,	// Right 
    -0.5f, -0.5f, 0.0f,	// Left
};

constexpr auto vert_colors = std::array {
    1.0f, 0.0f, 0.0f,	// Top
    0.0f, 1.0f, 0.0f,	// Right 
    0.0f, 0.0f, 1.0f	// Left
};

class Window final : public tinygl::Window
{
public:
    using tinygl::Window::Window;
    void init() override;
    void draw() override;
private:
    tinygl::Buffer vbo_pos{tinygl::Buffer::Type::VertexBuffer, tinygl::Buffer::UsagePattern::StaticDraw};
    tinygl::Buffer vbo_color{tinygl::Buffer::Type::VertexBuffer, tinygl::Buffer::UsagePattern::StaticDraw};
    tinygl::VertexArrayObject vao{};
    tinygl::ShaderProgram program;
    void showFPS();
};

void Window::init()
{
     setKeyCallback([this](tinygl::keyboard::Key key, int /*scancode*/, tinygl::input::Action action, tinygl::input::Modifier /*mods*/) {
        if (key == tinygl::keyboard::Key::Escape && action == tinygl::input::Action::Press) {
            setShouldClose(true);
        }
    });

    vbo_pos.bind();
    vbo_pos.create(vert_pos.begin(), vert_pos.end());

    vbo_color.bind();
    vbo_color.create(vert_colors.begin(), vert_colors.end());

    vao.bind();

    vbo_pos.bind();
    vao.setAttributeArray(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    vao.enableAttributeArray(0);

    vbo_color.bind();
    vao.setAttributeArray(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    vao.enableAttributeArray(1);

    program.addShaderFromSourceFile(tinygl::Shader::Type::Vertex, "04-02.vert");
    program.addShaderFromSourceFile(tinygl::Shader::Type::Fragment, "04-02.frag");
    program.link();

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
}

void Window::draw()
{
    showFPS();
    glClear(GL_COLOR_BUFFER_BIT);
    program.use();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vao.unbind();
}

void Window::showFPS()
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;

    double currentSeconds = glfwGetTime();
    double elapsedSeconds = currentSeconds - previousSeconds;
    frameCount++;

    if (elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double frameTime = 1000.0 / fps;

        std::ostringstream oss;
        oss.precision(3);
        oss << std::fixed << windowTitle << "    " << "FPS: " << fps << "    " << "Frame Time: " << frameTime << " (ms)";
        setTitle(oss.str());
        frameCount = 0;
    }
}

MAIN(3, 3, 800, 600)
