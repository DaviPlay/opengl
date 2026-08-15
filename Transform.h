#ifndef OPENGL_TRANSFORM_H
#define OPENGL_TRANSFORM_H
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

struct Transform
{
    glm::vec3 position { 0 };
    glm::vec3 rotation { 0 };
    glm::vec3 scale { 1 };

    [[nodiscard]] glm::mat4 matrix() const
    {
        glm::mat4 m { glm::translate(glm::mat4(1.0f), position) };
        m = glm::rotate(m, glm::radians(rotation.x), { 1, 0, 0 });
        m = glm::rotate(m, glm::radians(rotation.y), { 0, 1, 0 });
        m = glm::rotate(m, glm::radians(rotation.z), { 0, 0, 1 });
        m = glm::scale(m, scale);
        return m;
    }
};
#endif
