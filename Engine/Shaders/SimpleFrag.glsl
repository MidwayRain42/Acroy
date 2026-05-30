#version 460 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTexCoord;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform PerFrameData
{
    mat4 model;
    mat4 view;
    mat4 proj;
    float time;
} perFrameData;

layout(std140, binding = 1) uniform Params
{
    vec4 color;
    bool useTexture;
} params;

layout(set = 0, binding = 0) uniform sampler2D texSampler;

void main()
{
    if (params.useTexture)
    {
        fragColor = texture(texSampler, vTexCoord);
    }
    else
    {
        fragColor = params.color;
    }
}