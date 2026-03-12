#include "GUIRender.h"

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

GUIRender::GUIRender(std::vector<Shader> &shaders, std::vector<Camera> &cameras, std::vector<Model> &models, std::vector<GUI> &gui_elements)
{
    this->shaders = shaders;
    this->cameras = cameras;
    this->models = models;
    this->gui_elements = gui_elements;
}

void GUIRender::ready3D() const
{
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const Shader &shader_3d = shaders.at(0);
    const Camera &main_camera = cameras.at(0);
    shader_3d.use();
    shader_3d.set_vec3("viewPos", main_camera.position);
    glm::mat4 projection { glm::perspective(glm::radians(main_camera.fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f) };
    shader_3d.set_mat4("projection", projection);
    glm::mat4 view { main_camera.get_view_matrix() };
    shader_3d.set_mat4("view", view);
    glm::mat4 shader_model { glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) };
    shader_model = glm::scale(shader_model, glm::vec3(0.5f, 0.5f, 0.5f));
    shader_3d.set_mat4("model", shader_model);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDepthFunc(GL_LESS);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
}

void GUIRender::ready2D() const
{
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();

    const Shader &shader_2d = shaders.at(1);
    shader_2d.use();
    //glOrtho(0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, -1.0, 1.0);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //glTranslatef(0.375, 0.375, 0.0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
}

void GUIRender::render3D() const
{
    this->ready3D();
    const Shader &shader_3d = shaders.at(0);
    const Model &backpack = models.at(0);

    backpack.draw(shader_3d);
}

void GUIRender::render2D() const
{
    this->ready2D();
    const Shader &shader_2d = shaders.at(1);
    const GUI &arm_stamina = gui_elements.at(0);

    arm_stamina.draw(shader_2d);
}
