#include "TextRenderer.h"

#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

TextRenderer::TextRenderer(const Shader &shader, unsigned int width, unsigned int height) : shader(shader)
{
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    setupBuffers();

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    shader.use();
    shader.set_mat4("projection", projection);
}

TextRenderer::~TextRenderer()
{
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void TextRenderer::load_font(const std::string &fontPath, unsigned int fontSize)
{
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            textureID,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair(c, character));
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void TextRenderer::render_text(const std::string &text, float x, float y, float scale, glm::vec3 color)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.use();
    shader.set_vec3("texColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (char c : text)
    {
        Character character = Characters[c];

        float xpos = x + character.bearing.x * scale;
        float ypos = y - (character.size.y - character.bearing.y) * scale;
        float w = character.size.x * scale;
        float h = character.size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, character.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (character.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::setupBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
