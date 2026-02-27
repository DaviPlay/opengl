#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <string>
#include <glm/fwd.hpp>

class Shader
{
public:
    Shader(const char* vertex_shader, const char* fragment_shader);

    void use() const;

    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_mat4(const std::string& name, glm::mat4& mat, int amount = 1) const;

private:
    unsigned int id;
};

#endif //OPENGL_SHADER_H