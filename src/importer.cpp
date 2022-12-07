#include "importer.h"

using namespace std;

namespace CG {
    void Importer::draw(Program& program) {
        for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].draw(program);
    }

    void Importer::loadModel(string path) {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            cout << "Assimp Error " << import.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void Importer::processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++) { processNode(node->mChildren[i], scene); }
    }

    Mesh Importer::processMesh(aiMesh* mesh, const aiScene* scene) {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        glm::vec4 color;

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            color = loadColor(material);
        }

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            // process vertex positions, normals, color
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
        }

        Mesh temp;
        temp.createMesh(vertices, indices, color);
        return temp;
    }

    void Importer::shutdown() {
        for (int i = 0; i < meshes.size(); i++) { meshes[i].shutdown(); }
    }

    glm::vec4 Importer::loadColor(aiMaterial* mat) {
        aiColor3D color(0.f, 0.f, 0.f);
        mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        return glm::vec4(color.r, color.g, color.b, 1.0f);
    }
}  // namespace CG
