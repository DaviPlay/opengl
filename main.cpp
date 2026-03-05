#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "GUIRender.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void render_loop(GLFWwindow* window, GUIRender& gui);
void process_input(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
unsigned int load_texture(const char *path);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

double last_x { SCR_WIDTH / 2 };
double last_y { SCR_HEIGHT / 2 };
double pitch, yaw { -90.0f };
bool firstMouse { true };

float delta_time;
float last_frame;

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
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(true);

    const Shader cube_shader("../Shaders/vertex.glsl", "../Shaders/frag.glsl");
    const Shader light_shader("../Shaders/light_vertex.glsl", "../Shaders/light_frag.glsl");

    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    cube_shader.use();
    cube_shader.set_vec3("dirLight.direction", -0.2f, 2.0f, -1.0f);
    cube_shader.set_vec3("dirLight.ambient", 0.25f, 0.25f, 0.25f);
    cube_shader.set_vec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
    cube_shader.set_vec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

    Shader shader_list[] {
        cube_shader,
        light_shader
    };

    GUIRender gui = GUIRender(shader_list, &camera);

    render_loop(window, gui);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);

    glfwTerminate();
    return 0;
}

void render_loop(GLFWwindow* window, GUIRender& gui)
{
    while(!glfwWindowShouldClose(window))
    {
        const float current_frame { static_cast<float>(glfwGetTime()) };
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        gui.render3D();
        gui.render2D();

        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(CameraAction::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(CameraAction::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(CameraAction::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(CameraAction::RIGHT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.process_keyboard(CameraAction::UP, delta_time);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.process_keyboard(CameraAction::DOWN, delta_time);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        camera.process_keyboard(CameraAction::SPRINT, delta_time);
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        camera.process_keyboard(CameraAction::WALK, delta_time);
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
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

    camera.process_mouse_movement(static_cast<float>(x_offset), static_cast<float>(y_offset), delta_time);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        camera.process_mouse_button(CameraAction::ZOOM);
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        camera.process_mouse_button(CameraAction::UNZOOM);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}