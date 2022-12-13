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
        /**
         * @brief the indices for drawing triangles
         */
        std::vector<GLuint> indices;
        glm::vec4 color;

        /**
         * @brief set up data for passing into OpenGL shaders
         * @param vertices
         * @param indices
         * @param color
         */
        void createMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, glm::vec4 color);
        /**
         * @brief generate a square on the xz plane at with center z
         * @param center
         * @param side
         * @param color
         */
        void createPlaneColor(glm::vec3 center, float side, glm::vec4 color);
        /**
         * @brief draw all the vertices as triangles with help of glDrawElements
         * @param program the shaders to use for drawing
         */
        void draw(Program& program);
        /**
         * @brief clean up vertex arrays and buffers
         */
        void shutdown();

    private:
        /**
         * @brief vertex array object
         */
        GLuint vao;
        /**
         * @brief vertex buffer object
         */
        GLuint vbo;
        /**
         * @brief element buffer object
         */
        GLuint ebo;
    };
}  // namespace CG
