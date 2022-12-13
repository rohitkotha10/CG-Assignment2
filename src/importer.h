#pragma once
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace CG {
    class Importer {
    public:
        /**
         * @brief process the model from path and triangulate it using assimp
         * @param path source directory
         */
        void loadModel(std::string path);
        /**
         * @brief draw all the meshes in the model
         * @param program the shader program to use for rendering
         */
        void draw(Program& program);
        /**
         * @brief clean up all the meshes in the model
         */
        void shutdown();

    private:
        /**
         * @brief the model is split into many meshes
         */
        std::vector<Mesh> meshes;
        /**
         * @brief source directory
         */
        std::string directory;

        /**
         * @brief start processing node recursively
         * @param node each node has set of child nodes and a set of meshes in it
         * @param scene the source for our whole model generated by assimp
         */
        void processNode(aiNode* node, const aiScene* scene);
        /**
         * @brief generate vertices, textures, color and save it
         * @param mesh the mesh generated by assimp
         * @param scene the source for our whole model generated by assimp
         * @return mesh
         */
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        /**
         * @brief load the diffuse color only from material
         * @param mat the assimp material
         * @return the color vector
         */
        glm::vec4 loadColor(aiMaterial* mat);
    };
}  // namespace CG
