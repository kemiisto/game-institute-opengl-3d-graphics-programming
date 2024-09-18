cmake_minimum_required(VERSION 3.20)

project(game_institute_opengl_3d_graphics_programming)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(tinygl)
include_directories(. tinygl/include tinygl/imgui tinygl/tinyla/include)
link_libraries(tinygl)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
    add_compile_options(-Wno-gnu-anonymous-struct -Wno-nested-anon-types)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W3 /WX)
endif()

set(CHAPTERS
    03
    04
    05
    06
    08
)

function(create_project_from_sources chapter demo)
    get_filename_component(NAME "${demo}" NAME_WE)
    set(NAME ${chapter}-${NAME})
    message(STATUS "Configuring example ${NAME}")
    add_executable(${NAME} ${demo})
    target_compile_definitions(${NAME} PRIVATE NAME="${NAME}")
    # copy shaders and textures to build directory
    file(GLOB RESOURCES
        "${chapter}/*.vert"
        "${chapter}/*.tesc"
        "${chapter}/*.tese"
        "${chapter}/*.frag"
        "${chapter}/*.geom"
        "${chapter}/*.jpg"
        "${chapter}/*.png"
    )
    foreach(RESOURCE ${RESOURCES})
        add_custom_command(TARGET ${NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${RESOURCE}
                $<TARGET_FILE_DIR:${NAME}>
        )
    endforeach(RESOURCE)
    add_custom_command(TARGET ${NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_BINARY_DIR}/fonts
            $<TARGET_FILE_DIR:${NAME}>/fonts
    )
endfunction()

foreach(CHAPTER ${CHAPTERS})
    message(STATUS "Configuring examples for chapter ${CHAPTER}")
    file(GLOB EXAMPLES ${CMAKE_SOURCE_DIR}/${CHAPTER}/*.cpp)
    foreach(EXAMPLE ${EXAMPLES})
        create_project_from_sources(${CHAPTER} ${EXAMPLE})
    endforeach(EXAMPLE)
endforeach(CHAPTER)
