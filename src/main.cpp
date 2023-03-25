#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "shader.h"

#define WIDTH 800
#define HEIGHT 800

static const GLfloat g_vertex_buffer_data[] = {
        //triangle 1
        -1.f, 1.f, 0.f,
        -1.f, -1.f, 0.f,
        1.f, -1.f, 0.f,

        //triangle 2
        1.f, 1.f, 0.f,
        1.f, -1.f, 0.f,
        -1.f, 1.f, 0.f
};

static glm::vec2 mouse_position = {0, 0};
static float mouse_scroll = 1.f;
static float mouse_zoom_norm = 1.f;

static bool is_dragging = false;
static glm::vec2 mouse_start_drag_position = {0, 0};
static glm::vec2 mouse_start_position = {0,0};

void glfw_mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    const float mouse_scroll_scale = -0.1f;

    mouse_scroll += mouse_scroll_scale * (1.f / mouse_zoom_norm) * (float)yoffset;
    mouse_zoom_norm = 1.f / mouse_scroll;
}

void glfw_mouse_input_callback(GLFWwindow *window, double x, double y) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        is_dragging = false;
        return;
    }

    if (!is_dragging) {
        is_dragging = true;
        mouse_start_drag_position = {(float) x, (float) y};
        mouse_start_position = mouse_position;
    }

    mouse_position = mouse_start_position + (glm::vec2{(float)x,(float)y} - mouse_start_drag_position);
}

int main() {
    glewExperimental = true;

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "failed to initialize glew" << std::endl;

        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertices;
    glGenBuffers(1, &vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint shader = LoadShaders("shaders/scene.vert", "shaders/scene.frag");

    glfwSetCursorPosCallback(window, glfw_mouse_input_callback);
    glfwSetScrollCallback(window, glfw_mouse_scroll_callback);

    do {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertices);
        glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void *) nullptr
        );

        {
            GLint resolution_location = glGetUniformLocation(shader, "resolution");
            glUniform2f(resolution_location, WIDTH, HEIGHT);
        }
        {
            GLint system_time_location = glGetUniformLocation(shader, "system_time");
            glUniform1d(system_time_location, glfwGetTime());
        }
        {
            GLint mouse_location = glGetUniformLocation(shader, "mouse");
            glUniform3f(mouse_location, -(mouse_position[0] / WIDTH) * mouse_zoom_norm, (mouse_position[1] / HEIGHT) * mouse_zoom_norm, mouse_scroll);
        }

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return 0;
}