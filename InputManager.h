#ifndef OPENGL_INPUTMANAGER_H
#define OPENGL_INPUTMANAGER_H

class InputManager {
private:
    std::unordered_map<InputAction, int> key_bindings;
    std::unordered_map<InputAction, bool> current_state;
    std::unordered_map<InputAction, bool> previous_state;
    GLFWwindow* window;

public:
    explicit InputManager(GLFWwindow* win);
    void update();
    bool is_action_just_pressed(InputAction action);
    bool is_action_held(InputAction action);
    bool is_action_released(InputAction action);
};

#endif
