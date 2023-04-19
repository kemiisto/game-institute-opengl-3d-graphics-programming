/**
 * Creates a basic resizeable window and OpenGL 3.3 context using GLFW.
 */
#include "main.h"
#include <tinygl/tinygl.h>
#include <iostream>

class Window final : public tinygl::Window
{
public:
    using tinygl::Window::Window;
    void draw() override;
};

void Window::draw()
{
    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

MAIN
