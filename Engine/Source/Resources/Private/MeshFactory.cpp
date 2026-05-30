#include "MeshFactory.hpp"
#include "Mesh.hpp"

namespace Acroy
{
    Mesh* MeshFactory::CreateQuad(f32 width, f32 height)
    {
        f32 vertices[] = {
            // positions           // colors         // texture coords
           -width,  height, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
           -width, -height, 0.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
            width, -height, 0.0f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
            width,  height, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f
        };

        u32 indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        MeshData meshData{};
        meshData.indexCount = 6;
        meshData.indices = indices;
        meshData.layout = VertexLayout::PosColorUV();
        meshData.verticesSize = sizeof(vertices);
        meshData.vertices = vertices;

        return new Mesh(meshData);
    }

    Mesh* MeshFactory::CreateCube(const glm::vec3& size)
    {
        const f32 hx = size.x * 0.5f;
        const f32 hy = size.y * 0.5f;
        const f32 hz = size.z * 0.5f;

        // Position               // Normal              // UV
        f32 vertices[] = {
            // Front
            -hx, -hy,  hz,         0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
             hx, -hy,  hz,         0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
             hx,  hy,  hz,         0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
            -hx,  hy,  hz,         0.0f,  0.0f,  1.0f,   0.0f, 1.0f,

            // Back
             hx, -hy, -hz,         0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
            -hx, -hy, -hz,         0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
            -hx,  hy, -hz,         0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
             hx,  hy, -hz,         0.0f,  0.0f, -1.0f,   0.0f, 1.0f,

            // Left
            -hx, -hy, -hz,        -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
            -hx, -hy,  hz,        -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
            -hx,  hy,  hz,        -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
            -hx,  hy, -hz,        -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,

            // Right
             hx, -hy,  hz,         1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
             hx, -hy, -hz,         1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
             hx,  hy, -hz,         1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
             hx,  hy,  hz,         1.0f,  0.0f,  0.0f,   0.0f, 1.0f,

            // Top
            -hx,  hy,  hz,         0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
             hx,  hy,  hz,         0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
             hx,  hy, -hz,         0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
            -hx,  hy, -hz,         0.0f,  1.0f,  0.0f,   0.0f, 1.0f,

            // Bottom
            -hx, -hy, -hz,         0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
             hx, -hy, -hz,         0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
             hx, -hy,  hz,         0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
            -hx, -hy,  hz,         0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        };

        u32 indices[] = {
             0,  1,  2,   0,  2,  3, // Front
             4,  5,  6,   4,  6,  7, // Back
             8,  9, 10,   8, 10, 11, // Left
            12, 13, 14,  12, 14, 15, // Right
            16, 17, 18,  16, 18, 19, // Top
            20, 21, 22,  20, 22, 23  // Bottom
        };

        MeshData meshData{};
        meshData.indexCount = 36;
        meshData.indices = indices;
        meshData.layout = VertexLayout::PosNormalUV();
        meshData.verticesSize = sizeof(vertices);
        meshData.vertices = vertices;

        return new Mesh(meshData);
    }
}
