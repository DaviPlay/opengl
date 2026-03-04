#include "Camera.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), fov(FOV)
{
    this->position = position;
    this->world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;

    update_camera_vectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), fov(FOV)
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

void Camera::process_keyboard(CameraAction direction, const float& delta_time)
{
    const float velocity { movement_speed * delta_time };
    const glm::vec3 move_front { glm::normalize(glm::vec3(front.x, 0.0f, front.z)) };

    switch (direction)
    {
        case CameraAction::FORWARD: position += move_front * velocity; break;
        case CameraAction::BACKWARD: position -= move_front * velocity; break;
        case CameraAction::LEFT: position -= right * velocity; break;
        case CameraAction::RIGHT: position += right * velocity; break;
        case CameraAction::UP: position += up * velocity; break;
        case CameraAction::DOWN: position -= up * velocity; break;
        case CameraAction::SPRINT: movement_speed *= 2.0f; break;
        case CameraAction::WALK: movement_speed /= 2.0f; break;
        default: break;
    }
}

void Camera::process_mouse_movement(float x_offset, float y_offset, const float& delta_time, GLboolean constrain_pitch)
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

void Camera::process_mouse_button(CameraAction camera_action)
{
    switch (camera_action)
    {
        case CameraAction::ZOOM: fov -= ZOOM_AMOUNT; break;
        case CameraAction::UNZOOM: fov += ZOOM_AMOUNT; break;
        default: break;
    }
}

void Camera::update_camera_vectors()
{
    glm::vec3 direction;
    direction.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    direction.y = static_cast<float>(sin(glm::radians(pitch)));
    direction.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}
