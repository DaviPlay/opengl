#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraAction
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ZOOM,
    UNZOOM,
    SPRINT,
    WALK
};

constexpr float YAW { -90.0f };
constexpr float PITCH { 0.0f };
constexpr float SPEED { 2.5f };
constexpr float SENSITIVITY { 15.0f };
constexpr float FOV { 45.0f };
constexpr float ZOOM_AMOUNT { 20.0f };

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;
    float movement_speed;
    float mouse_sensitivity;
    float fov;

    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    explicit Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    [[nodiscard]] glm::mat4 get_view_matrix() const;
    void process_keyboard(CameraAction direction, const float& delta_time);
    void process_mouse_movement(float x_offset, float y_offset, const float& delta_time, GLboolean constrain_pitch = true);
    void process_mouse_scroll(float y_offset);
    void process_mouse_button(CameraAction camera_action);

private:
    void update_camera_vectors();
};

#endif //OPENGL_CAMERA_H