#ifndef OPENGL_GUIRENDER_H
#define OPENGL_GUIRENDER_H

#include "Camera.h"
#include "Shader.h"

class GUIRender
{
public:
    explicit GUIRender(Shader* shader, Camera* camera);

    void render2D() const;
    void render3D() const;

private:
    const Shader* shaders;
    const Camera* camera;

    void ready2D() const;
    void ready3D() const;
};

#endif