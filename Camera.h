#ifndef OPENGL_CAMERA_H
#define OPENGL_CAMERA_H

#include <glm/gtc/matrix_transform.hpp>

enum class InputAction
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
    WALK,
    EXIT
};

constexpr float YAW { -90.0f };
constexpr float PITCH { 0.0f };
constexpr float SPEED { 2.5f };
constexpr float SENSITIVITY { 15.0f };
constexpr float FOV { 45.0f };
constexpr float ZOOM_AMOUNT { 20.0f };

constexpr float ARM_MAX_STAMINA { 100.0f };
constexpr float ARM_MIN_STAMINA_TO_ZOOM { 15.0f };
constexpr float ARM_DRAIN_RATE { 35.0f };   // per second while sprinting
constexpr float ARM_REGEN_RATE { 20.0f };   // per second otherwise

constexpr float RUN_MAX_STAMINA { 100.0f };
constexpr float RUN_MIN_STAMINA_TO_SPRINT { 10.0f };
constexpr float RUN_DRAIN_RATE { 15.0f };   // per second while sprinting
constexpr float RUN_REGEN_RATE { 5.0f };   // per second otherwise

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
    float arm_stamina;
    float run_stamina;

    bool is_zooming;
    bool is_sprinting;
    bool is_walking;
    bool is_still;

    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    explicit Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    [[nodiscard]] glm::mat4 get_view_matrix() const;
    void process_keyboard(InputAction direction, const float& delta_time);
    void process_mouse_movement(float x_offset, float y_offset, const float& delta_time, bool constrain_pitch = true);
    void process_mouse_scroll(float y_offset);
    void process_mouse_button(InputAction camera_action, const float& delta_time);

private:
    void update_camera_vectors();
};

#endif