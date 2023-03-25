#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


static bool GetFileContents(const std::string &file_path, std::string &out_contents) {
    std::ifstream stream(file_path, std::ios::in);
    if (!stream.is_open()) {
        std::cout << "failed to load file:" << file_path << std::endl;
        return false;
    }

    std::stringstream ss;
    ss << stream.rdbuf();

    out_contents = ss.str();

    return true;
}

bool GenerateShader(GLuint shaderid, const std::string &file_path) {
    std::string shader_code;
    if (!GetFileContents(file_path, shader_code)) {
        return false;
    }

    std::cout << "Compiling shader: " << file_path << std::endl;

    const char *cshader_code = shader_code.c_str();
    glShaderSource(shaderid, 1, &cshader_code, nullptr);
    glCompileShader(shaderid);

    GLint result;
    int info_length;
    glGetShaderiv(shaderid, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &info_length);
    if (info_length > 0) {
        std::vector<char> shader_error_message(info_length + 1);
        glGetShaderInfoLog(shaderid, info_length, nullptr, &shader_error_message[0]);
        std::cout << &shader_error_message[0] << std::endl;

        return false;
    }

    return true;
}

GLuint LoadShaders(const std::string &vertex_file_path, const std::string &fragment_file_path) {

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    if (!GenerateShader(vertex_shader, vertex_file_path)) {
        return 0;
    }

    if (!GenerateShader(fragment_shader, fragment_file_path)) {
        return 0;
    }

    // Link the program
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vertex_shader);
    glAttachShader(ProgramID, fragment_shader);
    glLinkProgram(ProgramID);

    // Check the program
    GLint result;
    int info_length;
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &info_length);
    if (info_length > 0) {
        std::vector<char> ProgramErrorMessage(info_length + 1);
        glGetProgramInfoLog(ProgramID, info_length, NULL, &ProgramErrorMessage[0]);
        std::cout << &ProgramErrorMessage[0] << std::endl;
    }

    glDetachShader(ProgramID, vertex_shader);
    glDetachShader(ProgramID, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return ProgramID;
}