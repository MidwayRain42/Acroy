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
}
