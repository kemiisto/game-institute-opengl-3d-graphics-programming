/**
 * Adds full screen support.
 */
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const char* windowTitle = "Introduction to Modern OpenGL - Hello Window 4";
const int windowWidth = 800;
const int windowHeight = 600;

void onKey(GLFWwindow* window, int key, int scancode, int action, int mode);\
void onFramebufferSize(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);

int main()
{
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = nullptr;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
    if (vidmode) {
        window = glfwCreateWindow(vidmode->width, vidmode->height, windowTitle, monitor, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW mainWindow" << std::endl;
            glfwTerminate();
            return EXIT_FAILURE;
        }
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetKeyCallback(window, onKey);
    glfwSetFramebufferSizeCallback(window, onFramebufferSize);

    const auto renderer = glGetString(GL_RENDERER);
    const auto version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version: " << version << std::endl;
    std::cout << "OpenGL Initialization Completed!" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        showFPS(window);

        glfwPollEvents();

        glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void showFPS(GLFWwindow* window)
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
        glfwSetWindowTitle(window, oss.str().c_str());
        frameCount = 0;
    }
}

void onFramebufferSize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
