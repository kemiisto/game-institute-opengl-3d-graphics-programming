/**
 * Add another texture.
 */
#include "main.h"
#include <tinygl/tinygl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <iostream>
#include <sstream>

const char* windowTitle = NAME;
bool wireframe = false;

constexpr auto vertices = std::array {
        // position      // tex coords

        // front face
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

        // back face
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

        // left face
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

        // right face
         1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

        // top face
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

        // bottom face
        -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
};

class Window final : public tinygl::Window
{
public:
    using tinygl::Window::Window;
    void init() override;
    void draw() override;
private:
    tinygl::Buffer vbo{tinygl::Buffer::Type::VertexBuffer, tinygl::Buffer::UsagePattern::StaticDraw};
    tinygl::VertexArrayObject vao{};
    tinygl::Texture texture0{tinygl::Texture::Target::Target2D, "wooden_crate.jpg", GL_RGB, GL_RGB, true, 0};
    tinygl::ShaderProgram program;
    glm::vec3 cubePos{0.0f, 0.0f, -5.0f};
    float cubeAngle{};
    float lastTime{};
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
    // position attribute
    vao.setAttributeArray(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    vao.enableAttributeArray(0);

    // texture coord attribute
    vao.setAttributeArray(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));
    vao.enableAttributeArray(1);

    program.addShaderFromSourceFile(tinygl::Shader::Type::Vertex, "08-01.vert");
    program.addShaderFromSourceFile(tinygl::Shader::Type::Fragment, "08-01.frag");
    program.link();

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    lastTime = tinygl::getTime<float>();
}

void Window::draw()
{
    showFPS();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto currentTime = tinygl::getTime<float>();
    auto deltaTime = currentTime - lastTime;
    cubeAngle += deltaTime * 50.0f;
    if (cubeAngle >= 360.0f) cubeAngle = 0.0f;

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, cubePos) * glm::rotate(model, glm::radians(cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    auto camPos = glm::vec3{0.0f, 0.0f, 0.0f};
    auto targetPos = glm::vec3{0.0f, 0.0f, -1.0f};
    auto up = glm::vec3{0.0f, 1.0f, 0.0f};
    auto view = glm::lookAt(camPos, camPos + targetPos, up);

    auto projection = glm::perspective(glm::radians(45.0f), aspectRatio<float>(), 0.1f, 100.0f);

    texture0.bind();
    
    program.use();
    program.setUniformValue("texSampler0", 0);
    program.setUniformValue("model", glm::value_ptr(model));
    program.setUniformValue("view", glm::value_ptr(view));
    program.setUniformValue("projection", glm::value_ptr(projection));
    
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    vao.unbind();

    lastTime = currentTime;
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

MAIN
