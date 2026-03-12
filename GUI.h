#ifndef OPENGL_GUI_H
#define OPENGL_GUI_H

#include "Shader.h"

class GUI
{
public:
    const float* vertices;
    const unsigned int* indices;

    explicit GUI(const float* vertices, const unsigned int* indices);

    void draw(const Shader& shader) const;

private:
    unsigned int VAO, VBO, EBO;

    void setup_element();
};

#endif