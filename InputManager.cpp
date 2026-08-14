#include <ranges>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "InputManager.h"

InputManager::InputManager(GLFWwindow* win) : window(win) {
    key_bindings[InputAction::FORWARD] = GLFW_KEY_W;
    key_bindings[InputAction::BACKWARD] = GLFW_KEY_S;
    key_bindings[InputAction::LEFT] = GLFW_KEY_A;
    key_bindings[InputAction::RIGHT] = GLFW_KEY_D;
    key_bindings[InputAction::UP] = GLFW_KEY_SPACE;
    key_bindings[InputAction::DOWN] = GLFW_KEY_LEFT_CONTROL;
    key_bindings[InputAction::SPRINT] = GLFW_KEY_LEFT_SHIFT;
    key_bindings[InputAction::ZOOM] = GLFW_MOUSE_BUTTON_RIGHT;
    key_bindings[InputAction::EXIT] = GLFW_KEY_ESCAPE;

    // Initialize states to false
    for (const auto &key: key_bindings | std::views::keys) {
        current_state[key] = false;
        previous_state[key] = false;
    }
}

void InputManager::update() {
    for (const auto&[fst, snd] : key_bindings) {
        InputAction action = fst;
        const int key = snd;

        // 1. Save last frame's state
        previous_state[action] = current_state[action];

        // 2. Get this frame's state
        current_state[action] = (glfwGetKey(window, key) == GLFW_PRESS);
    }
}

bool InputManager::is_action_just_pressed(InputAction action) {
    return current_state[action] && !previous_state[action];
}

bool InputManager::is_action_held(InputAction action) {
    return current_state[action];
}

bool InputManager::is_action_released(InputAction action) {
    return glfwGetKey(window, key_bindings[action]) == GLFW_RELEASE;
}
