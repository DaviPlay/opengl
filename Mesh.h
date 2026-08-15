#ifndef MESHLOADING_H
#define MESHLOADING_H

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    //bone indexes that will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    explicit Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
    ~Mesh();
    Mesh(Mesh& model) = delete;
    Mesh& operator=(Mesh& model) = delete;
    Mesh(Mesh&& other) noexcept :
    vertices(std::move(other.vertices)),
    indices(std::move(other.indices)),
    textures(std::move(other.textures)),
    VAO(other.VAO),
    VBO(other.VBO),
    EBO(other.EBO)
    {
        other.VAO = other.VBO = other.EBO = 0;
    }

    // 2. Update the Move Assignment Operator to include the vectors
    Mesh& operator=(Mesh&& other) noexcept {
        if (this != &other)
        {
            // Clean up current buffers
            if (VAO != 0) glDeleteVertexArrays(1, &VAO);
            if (VBO != 0) glDeleteBuffers(1, &VBO);
            if (EBO != 0) glDeleteBuffers(1, &EBO);

            // Move the vectors
            vertices = std::move(other.vertices);
            indices  = std::move(other.indices);
            textures = std::move(other.textures);

            // Steal the buffers
            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;

            // Nullify the source
            other.VAO = 0;
            other.VBO = 0;
            other.EBO = 0;
        }
        return *this;
    }

    void draw(const Shader& shader) const;

private:
    unsigned int VAO{}, VBO{}, EBO{};

    void setup_mesh();
};
#endif