#include "Shader.h"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>

Shader::Shader(const char* vertex_shader, const char* fragment_shader) : id()
{
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    namespace fs = std::filesystem;
    auto resolve = [](const char* p) {
        fs::path path(p);
        if (path.is_absolute() || fs::exists(path)) return path;
        // fall back to path relative to current executable / project
        return fs::absolute(path);
    };
    const fs::path v_path = resolve(vertex_shader);
    const fs::path f_path = resolve(fragment_shader);

    try
    {
        v_shader_file.open(v_path);
        f_shader_file.open(f_path);

        std::stringstream v_shader_stream, f_shader_stream;
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        v_shader_file.close();
        f_shader_file.close();

        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();

        auto strip_bom = [](std::string& s) {
            if (s.size() >= 3 &&
                static_cast<unsigned char>(s[0]) == 0xEF &&
                static_cast<unsigned char>(s[1]) == 0xBB &&
                static_cast<unsigned char>(s[2]) == 0xBF)
            {
                s.erase(0, 3);
            }
        };
        auto normalize_newlines = [](std::string& s) {
            std::erase(s, '\r');
        };

        strip_bom(vertex_code);
        strip_bom(fragment_code);
        normalize_newlines(vertex_code);
        normalize_newlines(fragment_code);
    }
    catch ([[maybe_unused]] std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();

    auto check_shader = [](GLuint sh, const char* stage, const char* path) {
        GLint ok = GL_FALSE;
        glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
        GLint len = 0;
        glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &len);
        if (len > 1) {
            std::string log(static_cast<size_t>(len), '\0');
            glGetShaderInfoLog(sh, len, nullptr, log.data());
            std::cout << (ok ? "[shader log] " : "ERROR::SHADER::")
                      << stage << " (" << path << ")\n" << log << std::endl;
        }
        return ok == GL_TRUE;
    };
    auto check_program = [](GLuint pr) {
        GLint ok = GL_FALSE;
        glGetProgramiv(pr, GL_LINK_STATUS, &ok);
        GLint len = 0;
        glGetProgramiv(pr, GL_INFO_LOG_LENGTH, &len);
        if (len > 1) {
            std::string log(static_cast<size_t>(len), '\0');
            glGetProgramInfoLog(pr, len, nullptr, log.data());
            std::cout << (ok ? "[program log]\n" : "ERROR::SHADER::PROGRAM::LINKING_FAILED\n")
                      << log << std::endl;
        }
        return ok == GL_TRUE;
    };

    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_code, nullptr);
    glCompileShader(vertex);
    check_shader(vertex, "VERTEX", vertex_shader);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_code, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    check_shader(fragment, "FRAGMENT", fragment_shader);

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    check_program(id);

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
