#include "GUIRender.h"

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Model.h"

glm::mat4 perspective;

const Model backpack("../Assets/backpack/backpack.obj");

GUIRender::GUIRender(Shader* shaders, Camera* camera)
{
    this->shaders = shaders;
    this->camera = camera;
}

void GUIRender::ready3D() const
{
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    perspective = glm::perspective(glm::radians(camera->fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDepthFunc(GL_EQUAL);
    glEnable(GL_DEPTH_TEST);
}

void GUIRender::ready2D() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.375, 0.375, 0.0);

    glDisable(GL_DEPTH_TEST);
}

void GUIRender::render3D() const
{
    this->ready3D();

    const Shader& cubes_shader = shaders[0];
    cubes_shader.use();
    cubes_shader.set_vec3("viewPos", camera->position);
    glm::mat4 projection { perspective };
    cubes_shader.set_mat4("projection", projection);
    glm::mat4 view { camera->get_view_matrix() };
    cubes_shader.set_mat4("view", view);
    glm::mat4 shader_model { glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) };
    shader_model = glm::scale(shader_model, glm::vec3(0.5f, 0.5f, 0.5f));
    cubes_shader.set_mat4("model", shader_model);

    backpack.draw(shaders[0]);
}

void GUIRender::render2D() const
{
    this->ready2D();
}
