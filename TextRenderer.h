#ifndef OPENGL_TEXTRENDERER_H
#define OPENGL_TEXTRENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <map>
#include <string>

#include "Shader.h"

struct Character
{
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class TextRenderer
{
public:
    TextRenderer(Shader shader, unsigned int width, unsigned int height);
    ~TextRenderer();

    void load_font(const std::string& fontPath, unsigned int fontSize);
    void render_text(const std::string& text, float x, float y, float scale, glm::vec3 color);

private:
    Shader shader;
    unsigned int VAO, VBO;
    std::map<char, Character> Characters;
    FT_Library ft;
    FT_Face face;

    void setupBuffers();
};

#endif
