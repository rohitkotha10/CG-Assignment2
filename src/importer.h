#pragma once
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace CG {

    class Importer {
    public:
        void loadModel(std::string path);
        void draw(Program& program);
        void shutdown();

    private:
        std::vector<Mesh> meshes;
        std::string directory;

        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        glm::vec4 loadColor(aiMaterial* mat);
    };

}  // namespace CG
