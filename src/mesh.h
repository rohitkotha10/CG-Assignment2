#pragma once
#include "program.h"

namespace CG {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
    };

    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        glm::vec4 color;

        void createMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, glm::vec4 color);
        void createPlaneColor(glm::vec3 center, float side, glm::vec4 color);
        void draw(Program& program);
        void shutdown();

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
    };
}  // namespace CG
