#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "GUIRender.h"
#include "Model.h"
#include "TextRenderer.h"
#include "InputManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_refresh_callback(GLFWwindow* window);
void window_pos_callback(GLFWwindow* window, int x, int y);
void render_loop(GLFWwindow* window, const GUIRender& gui, InputManager& input_manager);
static void render_frame(GLFWwindow* window);
void process_input(GLFWwindow* window, InputManager& input_manager);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
unsigned int load_texture(const char *path);

static std::vector<Camera> camera_list;

const GUIRender* g_gui = nullptr;

double last_x { SCR_WIDTH / 2 };
double last_y { SCR_HEIGHT / 2 };
double pitch, yaw { -90.0f };
bool firstMouse { true };

float delta_time;
float last_frame;

constexpr float VERTICES_ARM_STAMINA_OUTLINE[] = {
    // positions          // colors           // texture coords
    0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.7f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
   -0.5f, -0.7f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
   -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};
constexpr float VERTICES_ARM_STAMINA[] = {
    // positions          // colors           // texture coords
    0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.7f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
   -0.5f, -0.7f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
   -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};
constexpr unsigned int INDICES[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window { glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr) };
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowRefreshCallback(window, window_refresh_callback);
    glfwSetWindowPosCallback(window, window_pos_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    stbi_set_flip_vertically_on_load(true);

    Shader shader_3d("../Shaders/vertex.glsl", "../Shaders/frag.glsl");
    Shader shader_2d("../Shaders/gui_vertex.glsl", "../Shaders/gui_frag.glsl");
    Shader shader_text("../Shaders/text_vertex.glsl", "../Shaders/text_frag.glsl");
    Shader light_shader("../Shaders/light_vertex.glsl", "../Shaders/light_frag.glsl");

    Camera main_camera(glm::vec3(0.0f, 0.0f, 3.0f));

    TextRenderer text_renderer((std::move(shader_text)), SCR_WIDTH, SCR_HEIGHT);
    text_renderer.load_font("../Assets/arial.ttf", 48);

    Model backpack("../Assets/backpack/backpack.obj");

    GUI arm_stamina_outline(VERTICES_ARM_STAMINA_OUTLINE, INDICES, sizeof(VERTICES_ARM_STAMINA_OUTLINE) / sizeof(float));
    GUI arm_stamina(VERTICES_ARM_STAMINA, INDICES, sizeof(VERTICES_ARM_STAMINA) / sizeof(float));

    //unsigned int fbo;
    //glGenFramebuffers(1, &fbo);
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    camera_list.reserve(1);
    camera_list.emplace_back(main_camera);

    std::vector<Shader> shader_list;
    shader_list.reserve(4);
    shader_list.push_back(std::move(shader_3d));
    shader_list.push_back(std::move(shader_2d));
    shader_list.push_back(std::move(light_shader));
    shader_list.push_back(std::move(shader_text));

    std::vector<Model> model_list;
    model_list.reserve(1);
    model_list.push_back(std::move(backpack));

    std::vector<GUI> gui_elements_list;
    gui_elements_list.reserve(2);
    gui_elements_list.push_back(std::move(arm_stamina_outline));
    gui_elements_list.push_back(std::move(arm_stamina));

    const GUIRender gui = GUIRender(std::move(shader_list), camera_list, std::move(model_list), std::move(gui_elements_list), text_renderer);
    g_gui = &gui;

    InputManager input_manager(window);

    render_loop(window, gui, input_manager);
    g_gui = nullptr;

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glDeleteFramebuffers(1, &fbo);

    glfwTerminate();
    return 0;
}

void render_loop(GLFWwindow* window, const GUIRender& gui, InputManager& input_manager)
{
    while(!glfwWindowShouldClose(window))
    {
        const float current_frame { static_cast<float>(glfwGetTime()) };
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        render_frame(window);

        input_manager.update();

        process_input(window, input_manager);

        Camera& camera = camera_list[0];
        if (camera.is_zooming)
            camera.arm_stamina -= ARM_DRAIN_RATE * delta_time;
        else
            camera.arm_stamina += ARM_REGEN_RATE * delta_time;

        camera.arm_stamina = std::clamp(camera.arm_stamina, 0.0f, ARM_MAX_STAMINA);

        if (camera.is_sprinting)
            camera.run_stamina -= RUN_DRAIN_RATE * delta_time;
        else
            camera.run_stamina += RUN_REGEN_RATE * delta_time;

        camera.run_stamina = std::clamp(camera.run_stamina, 0.0f, RUN_MAX_STAMINA);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void process_input(GLFWwindow *window, InputManager& input_manager)
{
    Camera& camera = camera_list[0];

    if (input_manager.is_action_held(InputAction::FORWARD))
        camera.process_keyboard(InputAction::FORWARD, delta_time);
    if (input_manager.is_action_held(InputAction::LEFT))
        camera.process_keyboard(InputAction::LEFT, delta_time);
    if (input_manager.is_action_held(InputAction::BACKWARD))
        camera.process_keyboard(InputAction::BACKWARD, delta_time);
    if (input_manager.is_action_held(InputAction::RIGHT))
        camera.process_keyboard(InputAction::RIGHT, delta_time);
    if (input_manager.is_action_held(InputAction::UP))
        camera.process_keyboard(InputAction::UP, delta_time);
    if (input_manager.is_action_held(InputAction::DOWN))
        camera.process_keyboard(InputAction::DOWN, delta_time);
    if (input_manager.is_action_just_pressed(InputAction::SPRINT) && camera.run_stamina > RUN_MIN_STAMINA_TO_SPRINT && !camera.is_sprinting)
        camera.process_keyboard(InputAction::SPRINT, delta_time);
    if (input_manager.is_action_released(InputAction::SPRINT) && camera.is_sprinting)
        camera.process_keyboard(InputAction::WALK, delta_time);

    if (input_manager.is_action_held(InputAction::ZOOM) && camera.arm_stamina > ARM_MIN_STAMINA_TO_ZOOM && !camera.is_zooming)
        camera.process_mouse_button(InputAction::ZOOM, delta_time);
    if (input_manager.is_action_released(InputAction::ZOOM) && camera.is_zooming)
        camera.process_mouse_button(InputAction::UNZOOM, delta_time);

    if (camera.arm_stamina <= 0.0f)
        camera.process_mouse_button(InputAction::UNZOOM, delta_time);

    if (camera.run_stamina <= 0.0f)
        camera.process_mouse_button(InputAction::WALK, delta_time);

    if (input_manager.is_action_held(InputAction::EXIT))
        glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    if (camera_list.empty()) return;

    if (firstMouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        firstMouse = false;
    }

    const double x_offset { x_pos - last_x };
    const double y_offset { last_y - y_pos };
    last_x = x_pos;
    last_y = y_pos;

    camera_list[0].process_mouse_movement(static_cast<float>(x_offset), static_cast<float>(y_offset), delta_time);
}

static void render_frame(GLFWwindow* window)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_gui->render3D(0);
    g_gui->render2D();
}

void window_refresh_callback(GLFWwindow* window)
{
    if (!g_gui) return;
    render_frame(window);
}

void window_pos_callback(GLFWwindow* window, int x, int y)
{
    firstMouse = true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (width <= 0 || height <= 0) return;
    glViewport(0, 0, width, height);
    if (!g_gui) return;
    render_frame(window);
}
