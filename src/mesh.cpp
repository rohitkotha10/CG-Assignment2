#include "mesh.h"

using namespace std;

namespace CG {
    void Mesh::createMesh(vector<Vertex> vertices, vector<GLuint> indices, glm::vec4 color) {
        this->vertices = vertices;
        this->indices = indices;
        this->color = color;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void Mesh::createPlaneColor(glm::vec3 center, float side, glm::vec4 color) {
        float hside = side / 2;

        Vertex a0;
        a0.position = glm::vec3(center.x - hside, center.y, center.z - hside);
        a0.normal = glm::vec3(0.0f, 1.0f, 0.0f);

        Vertex a1;
        a1.position = glm::vec3(center.x + hside, center.y, center.z - hside);
        a1.normal = glm::vec3(0.0f, 1.0f, 0.0f);

        Vertex a2;
        a2.position = glm::vec3(center.x + hside, center.y, center.z + hside);
        a2.normal = glm::vec3(0.0f, 1.0f, 0.0f);

        Vertex a3;
        a3.position = glm::vec3(center.x - hside, center.y, center.z + hside);
        a3.normal = glm::vec3(0.0f, 1.0f, 0.0f);

        vector<Vertex> vertices = {a0, a1, a2, a3};
        vector<GLuint> indices = {0, 1, 2, 2, 3, 0};

        this->createMesh(vertices, indices, color);
    }

    void Mesh::draw(Program& program) {
        glBindVertexArray(vao);
        program.setVec4("material.color", this->color);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Mesh::shutdown() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }
}  // namespace CG
