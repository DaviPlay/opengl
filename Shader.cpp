#include "Shader.h"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <bits/locale_facets_nonio.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>

Shader::Shader(const char* vertex_shader, const char* fragment_shader)
    : id()
{
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        v_shader_file.open(vertex_shader);
        f_shader_file.open(fragment_shader);

        std::stringstream v_shader_stream, f_shader_stream;
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        v_shader_file.close();
        f_shader_file.close();

        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
    }
    catch ([[maybe_unused]] std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();

    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_code, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(vertex, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << info_log << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_code, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(fragment, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << info_log << std::endl;
    }

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << info_log << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const
{
    glUseProgram(id);
}

void Shader::set_bool(const std::string& name, const bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::set_int(const std::string& name, const int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::set_float(const std::string& name, const float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::set_vec3(const std::string &name, const glm::vec3& vec, const int amount) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), amount, glm::value_ptr(vec));
}
void Shader::set_vec3(const std::string &name, float x, float y, float z, const int amount) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), amount, glm::value_ptr(glm::vec3(x, y, z)));
}
void Shader::set_mat4(const std::string& name, glm::mat4& mat, const int amount) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), amount, GL_FALSE, glm::value_ptr(mat));
}
