#version 460 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTexCoord;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 2) uniform Params
{
    vec4 color;
    bool useTexture;
    vec2 uvScale;
} params;

layout(set = 0, binding = 0) uniform sampler2D texSampler;

void main()
{
    if (params.useTexture)
    {
        vec2 uvScale = vec2(
            params.uvScale.x == 0.0 ? 1.0 : params.uvScale.x,
            params.uvScale.y == 0.0 ? 1.0 : params.uvScale.y
        );
        fragColor = texture(texSampler, vTexCoord * uvScale);
    }
    else
    {
        fragColor = params.color;
    }
}