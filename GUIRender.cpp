#include "GUIRender.h"

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

GUIRender::GUIRender(const Shader** shaders, const Camera** cameras, const Model** models)
{
    this->shaders = *shaders;
    this->cameras = *cameras;
    this->models = *models;
}

void GUIRender::ready3D() const
{
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const Shader& cubes_shader = shaders[0];
    cubes_shader.use();
    cubes_shader.set_vec3("viewPos", cameras[0].position);
    glm::mat4 projection { glm::perspective(glm::radians(cameras[0].fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f) };
    cubes_shader.set_mat4("projection", projection);
    glm::mat4 view { cameras[0].get_view_matrix() };
    cubes_shader.set_mat4("view", view);
    glm::mat4 shader_model { glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) };
    shader_model = glm::scale(shader_model, glm::vec3(0.5f, 0.5f, 0.5f));
    cubes_shader.set_mat4("model", shader_model);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDepthFunc(GL_LESS);

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

    models[0].draw(shaders[0]);
}

void GUIRender::render2D() const
{
    this->ready2D();
}
