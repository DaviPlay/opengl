#include "GUIRender.h"

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "TextRenderer.h"

GUIRender::GUIRender(std::vector<Shader> shaders, std::vector<Camera> cameras, std::vector<Model> models, std::vector<GUI> gui_elements, TextRenderer& text_renderer) : text_renderer(text_renderer)
{
    this->shaders = std::move(shaders);
    this->models = std::move(models);
    this->gui_elements = std::move(gui_elements);

    this->cameras.clear();
    for (auto& cam : cameras) {
        this->cameras.emplace_back(cam);
    }

    // CCW winding per face so outward normals are front-facing
    static constexpr float cube_verts[] = {
        // back  (normal -Z)
        -0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f, -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
        // front (normal +Z)
        -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        // left  (normal -X)
        -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f,
        // right (normal +X)
         0.5f,-0.5f, 0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
         0.5f,-0.5f, 0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,
        // bottom(normal -Y)
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f,
        // top   (normal +Y)
        -0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f,
    };
    glGenVertexArrays(1, &light_vao);
    glGenBuffers(1, &light_vbo);
    glBindVertexArray(light_vao);
    glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glBindVertexArray(0);
}

void GUIRender::ready3D(size_t camera_index) const
{
    if (camera_index >= cameras.size()) camera_index = 0;

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}

void GUIRender::ready2D() const
{
    const Shader &shader_2d = shaders[1];
    shader_2d.use();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
}

void GUIRender::render3D(const size_t camera_index) const
{
    this->ready3D(camera_index);
    const Shader& shader_3d = shaders[0];
    const Model& backpack = models[0];

    const Shader& light_cube_shader = shaders.at(2);
    const Camera& camera = cameras.at(camera_index).get();

    shader_3d.use();
    shader_3d.set_vec3("viewPos", camera.position);

    //lighting
    shader_3d.set_float("material.shininess", 256.0f);

    shader_3d.set_vec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader_3d.set_vec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader_3d.set_vec3("dirLight.diffuse", 0.2f, 0.2f, 0.2f);
    shader_3d.set_vec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    shader_3d.set_vec3("pointLights[0].position", 0.7f, 0.2f, 2.0f);
    shader_3d.set_vec3("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
    shader_3d.set_vec3("pointLights[0].diffuse", 0.9f, 0.9f, 0.9f);
    shader_3d.set_vec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader_3d.set_float("pointLights[0].constant", 1.0f);
    shader_3d.set_float("pointLights[0].linear", 0.09f);
    shader_3d.set_float("pointLights[0].quadratic", 0.032f);

    shader_3d.set_vec3("spotLight.position", camera.position);
    shader_3d.set_vec3("spotLight.direction", camera.front);
    shader_3d.set_vec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader_3d.set_vec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader_3d.set_vec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader_3d.set_float("spotLight.constant", 1.0f);
    shader_3d.set_float("spotLight.linear", 0.09f);
    shader_3d.set_float("spotLight.quadratic", 0.032f);
    shader_3d.set_float("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader_3d.set_float("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    const float aspect = static_cast<float>(vp[2]) / static_cast<float>(vp[3]);
    glm::mat4 projection { glm::perspective(glm::radians(camera.fov), aspect, 0.1f, 100.0f) };
    shader_3d.set_mat4("projection", projection);
    glm::mat4 view { camera.get_view_matrix() };
    shader_3d.set_mat4("view", view);
    glm::mat4 shader_model { glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) };
    shader_model = glm::scale(shader_model, glm::vec3(0.5f, 0.5f, 0.5f));
    shader_3d.set_mat4("model", shader_model);

    backpack.draw(shader_3d);

    // drawing lamps
    light_cube_shader.use();
    light_cube_shader.set_mat4("projection", projection);
    light_cube_shader.set_mat4("view", view);
    shader_model = glm::mat4(1.0f);
    shader_model = glm::translate(shader_model, glm::vec3(0.7f, 0.2f, 2.0f));
    shader_model = glm::scale(shader_model, glm::vec3(0.2f));
    light_cube_shader.set_mat4("model", shader_model);

    glBindVertexArray(light_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void GUIRender::render2D() const
{
    this->ready2D();
    const Shader &shader_2d = shaders.at(1);
    const GUI &arm_stamina_outline = gui_elements.at(0);
    const GUI &arm_stamina = gui_elements.at(1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    arm_stamina_outline.draw(shader_2d);
    text_renderer.render_text(std::to_string(static_cast<int>(cameras.at(0).get().arm_stamina)), 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f));
    text_renderer.render_text(std::to_string(static_cast<int>(cameras.at(0).get().run_stamina)), 150.0f, 25.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    auto model_fg = glm::mat4(1.0f);
    model_fg = glm::translate(model_fg, glm::vec3(0.0f, 0.0f, 0.0f));
    model_fg = glm::scale(model_fg, glm::vec3(300.0f * (cameras.at(0).get().arm_stamina / ARM_MAX_STAMINA), 20.0f, 1.0f));
    arm_stamina.draw(shader_2d, model_fg);
}
