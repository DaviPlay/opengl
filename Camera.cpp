#include "Camera.h"

#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : _front(glm::vec3(0.0f, 0.0f, -1.0f)), _movement_speed(SPEED), _zoom(ZOOM)
{
    _position = position;
    _up = up;
    _yaw = yaw;
    _pitch = pitch;

    update_camera_vectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
    _position = glm::vec3(posX, posY, posZ);
    _world_up = glm::vec3(upX, upY, upZ);
    _yaw = yaw;
    _pitch = pitch;

    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::process_keyboard(Camera_Movement direction, const float& delta_time)
{
    const float velocity = _movement_speed * delta_time;
    if (direction == FORWARD)
        _position += _front * velocity;
    if (direction == BACKWARD)
        _position -= _front * velocity;
    if (direction == LEFT)
        _position -= _right * velocity;
    if (direction == RIGHT)
        _position += _right * velocity;
}

void Camera::process_mouse_movement(float x_offset, float y_offset, const float& delta_time, GLboolean constrain_pitch)
{
    constexpr float sensitivity { 15.0f };
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    _yaw += x_offset * delta_time;
    if (_pitch < 89.0f && _pitch > -89.0f)
        _pitch += y_offset * delta_time;

    // update Front, Right and Up Vectors using the updated Euler angles
    update_camera_vectors();
}

void Camera::process_mouse_scroll(float y_offset) {}

void Camera::process_mouse_button(int button, int action)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        _zoom -= 20.0f;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        _zoom += 20.0f;
}

void Camera::update_camera_vectors()
{
    glm::vec3 direction;
    direction.x = static_cast<float>(cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)));
    direction.y = static_cast<float>(sin(glm::radians(_pitch)));
    direction.z = static_cast<float>(sin(glm::radians(_yaw)) * cos(glm::radians(_pitch)));

    _front = glm::normalize(direction);
    _right = glm::normalize(glm::cross(_front, _world_up));
    _up = glm::normalize(glm::cross(_right, _front));
}
