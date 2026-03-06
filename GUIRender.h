#ifndef OPENGL_GUIRENDER_H
#define OPENGL_GUIRENDER_H

#include "Camera.h"
#include "Model.h"
#include "Shader.h"

class GUIRender
{
public:
    explicit GUIRender(const Shader** shaders, const Camera** cameras, const Model** models);

    void render2D() const;
    void render3D() const;

private:
    const Shader* shaders;
    const Camera* cameras;
    const Model* models;

    void ready2D() const;
    void ready3D() const;
};

#endif