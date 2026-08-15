#ifndef OPENGL_ENTITY_H
#define OPENGL_ENTITY_H
#include "Transform.h"

struct Entity
{
    Transform transform;
    size_t model_index {};
    size_t shader_index {};
};

#endif
