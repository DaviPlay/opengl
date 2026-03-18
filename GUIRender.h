#ifndef OPENGL_GUIRENDER_H
#define OPENGL_GUIRENDER_H

#include "Camera.h"
#include "GUI.h"
#include "Model.h"
#include "Shader.h"

class GUIRender
{
public:
    explicit GUIRender(std::vector<Shader>& shaders, std::vector<Camera>& cameras, std::vector<Model>& models, std::vector<GUI>& gui_elements);

    void render2D() const;
    void render3D(size_t camera_index = 0) const;

private:
    std::vector<Shader> shaders;
    std::vector<std::reference_wrapper<const Camera>> cameras;
    std::vector<Model> models;
    std::vector<GUI> gui_elements;

    void ready2D() const;
    void ready3D(size_t camera_index) const;
};

#endif