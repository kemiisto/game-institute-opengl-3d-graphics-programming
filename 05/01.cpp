/**
 * Colored quad bouncing around the screen.
 */
#include <tinygl/main.h>
#include <tinygl/tinygl.h>
#include <array>
#include <iostream>
#include <sstream>

const char* windowTitle = NAME;
bool wireframe = false;

constexpr auto vertices = std::array {
    -0.5f,  0.5f, 0.0f,     // top left
     0.5f,  0.5f, 0.0f,     // top right
     0.5f, -0.5f, 0.0f,     // bottom right
    -0.5f, -0.5f, 0.0f      // bottom left
};

constexpr auto indices = std::to_array<GLuint>({
    0, 1, 2,  // first triangle
    0, 2, 3   // second triangle
});

class Window final : public tinygl::Window
{
public:
    using tinygl::Window::Window;
    void init() override;
    void draw() override;
private:
    tinygl::Buffer vbo{tinygl::Buffer::Type::VertexBuffer, tinygl::Buffer::UsagePattern::StaticDraw};
    tinygl::Buffer ibo{tinygl::Buffer::Type::IndexBuffer, tinygl::Buffer::UsagePattern::StaticDraw};
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
        if (key == tinygl::keyboard::Key::W && action == tinygl::input::Action::Press) {
            wireframe = !wireframe;
            if (wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
    });

    vbo.bind();
    vbo.create(vertices.begin(), vertices.end());

    vao.bind();
    vao.setAttributeArray(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    vao.enableAttributeArray(0);

    ibo.bind();
    ibo.create(indices.begin(), indices.end());

    program.addShaderFromSourceFile(tinygl::Shader::Type::Vertex, "05-01.vert");
    program.addShaderFromSourceFile(tinygl::Shader::Type::Fragment, "05-01.frag");
    program.link();

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
}

void Window::draw()
{
    showFPS();
    glClear(GL_COLOR_BUFFER_BIT);
    program.use();

    auto time = tinygl::getTime<float>();
    auto blueColor = (std::sin(time) / 2) + 0.5f;
    auto pos = tinyla::vec2f{std::sin(time) / 2, std::cos(time) / 2};
    program.setUniformValue("vertColor", tinyla::vec4f{0.0f, 0.0f, blueColor, 1.0f});
    program.setUniformValue("posOffset", pos);

    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    vao.unbind();
}

void Window::showFPS() {
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
