#include "Camera.h"

#include <iostream>
#include <glm/gtc/quaternion.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), fov(FOV), arm_stamina(ARM_MAX_STAMINA), run_stamina(RUN_MAX_STAMINA)
{
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;

    update_camera_vectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), fov(FOV), arm_stamina(ARM_MAX_STAMINA), run_stamina(RUN_MAX_STAMINA)
{
    this->position = glm::vec3(posX, posY, posZ);
    this->world_up = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;

    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::process_keyboard(const InputAction direction, const float& delta_time)
{
    const float velocity { movement_speed * delta_time };
    const glm::vec3 move_front { glm::normalize(glm::vec3(front.x, 0.0f, front.z)) };

    switch (direction)
    {
        case InputAction::FORWARD: position += move_front * velocity; break;
        case InputAction::BACKWARD: position -= move_front * velocity; break;
        case InputAction::LEFT: position -= right * velocity; break;
        case InputAction::RIGHT: position += right * velocity; break;
        case InputAction::UP: position += up * velocity; break;
        case InputAction::DOWN: position -= up * velocity; break;
        case InputAction::SPRINT:
        {
            movement_speed *= 2.0f;
            is_walking = false;
            is_sprinting = true;
            break;
        }
        case InputAction::WALK:
        {
            movement_speed /= 2.0f;
            is_sprinting = false;
            is_walking = true;
            break;
        }
        default: break;
    }
}

void Camera::process_mouse_movement(float x_offset, float y_offset, const float& delta_time, bool constrain_pitch)
{
    constexpr float sensitivity { 15.0f };
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset * delta_time;
    pitch += y_offset * delta_time;
    if (pitch > 89.9f)
        pitch = 89.9f;
    if (pitch < -89.9f)
        pitch = -89.9f;

    // update Front, Right and Up Vectors using the updated Euler angles
    update_camera_vectors();
}

void Camera::process_mouse_scroll(float y_offset) {}

void Camera::process_mouse_button(InputAction camera_action, const float& delta_time)
{
    switch (camera_action)
    {
        case InputAction::ZOOM:
        {
            fov -= ZOOM_AMOUNT;
            is_zooming = true;
            break;
        }
        case InputAction::UNZOOM:
        {
            fov += ZOOM_AMOUNT;
            is_zooming = false;
            break;
        }
        default: break;
    }
}

void Camera::update_camera_vectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}
