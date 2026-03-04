#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{
public:
    explicit Model(const char* path);
    void draw(const Shader& shader) const;

private:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    void load_model(const std::string &path);
    void process_node(const aiNode* node, const aiScene* scene);
    Mesh process_mesh(const aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, const std::string& type_name);
    static unsigned int texture_from_file(const char *path, const std::string &directory);
};

#endif