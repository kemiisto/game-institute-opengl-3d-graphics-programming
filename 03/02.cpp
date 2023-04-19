/**
 * Creates a basic resizeable window and OpenGL 3.3 context using GLFW.
 * Creates keyboard handler callback and sets the Esc key to exit the program.
 * Creates FPS counter and displays in app title bar.
 */
#include "main.h"
#include <tinygl/tinygl.h>
#include <iostream>
#include <sstream>

const char* windowTitle = NAME;

class Window final : public tinygl::Window
{
public:
    using tinygl::Window::Window;
    void init() override;
    void draw() override;
private:
    void showFPS();
};

void Window::init()
{
     setKeyCallback([this](tinygl::keyboard::Key key, int /*scancode*/, tinygl::input::Action action, tinygl::input::Modifier /*mods*/) {
        if (key == tinygl::keyboard::Key::Escape && action == tinygl::input::Action::Press) {
            setShouldClose(true);
        }
    });
}

void Window::draw()
{
    showFPS();
    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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
