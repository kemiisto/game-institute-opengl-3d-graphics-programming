cmake_minimum_required(VERSION 3.20)
project(game_institute_opengl_3d_graphics_programming)

set(CMAKE_CXX_STANDARD 20)

find_package(glew CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

link_libraries(GLEW::GLEW glfw)

add_subdirectory(03)
