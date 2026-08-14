#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/fwd.hpp>

class Shader
{
public:
    explicit Shader(const char* vertex_shader, const char* fragment_shader);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept : id(other.id)
    {
        other.id = 0;   // source no longer owns it, so its ~Shader() is a no-op
    }
    Shader& operator=(Shader&& other) noexcept
    {
        if (this != &other)
        {
            if (id != 0)
                glDeleteProgram(id);   // release our current resource first
            id = other.id;
            other.id = 0;
        }
        return *this;
    }

    void use() const;

    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec3(const std::string& name, const glm::vec3& vec, int amount = 1) const;
    void set_vec3(const std::string& name, float x, float y, float z, int amount = 1) const;
    void set_mat4(const std::string& name, glm::mat4& mat, int amount = 1) const;

private:
    unsigned int id;
};

#endif //OPENGL_SHADER_H