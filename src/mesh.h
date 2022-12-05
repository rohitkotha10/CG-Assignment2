#pragma once
#include "program.h"

#include <vector>

namespace CG {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
    };

    class Mesh {
    public:
        void createMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, glm::vec4 color);
        void draw(Program& program);
        void shutdown();

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        glm::vec4 color;

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
    };
}  // namespace CG
