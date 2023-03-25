cmake_minimum_required(VERSION 3.24)
set(VCPKG_TARGET_TRIPLET x64-windows-static)
project(mandelbrot)


find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(util)

add_executable(mandelbrot src/main.cpp shaders/scene.frag shaders/scene.vert)

target_link_libraries(mandelbrot PRIVATE glfw GLEW::GLEW glm::glm util)

add_custom_command(
        TARGET mandelbrot
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders
)