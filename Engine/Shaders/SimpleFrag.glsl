#version 460 core

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vTexCoord;

layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 0) uniform sampler2D texSampler;

layout(std140, binding = 0) uniform Params
{
    vec4 color;
    bool useTexture;
};

void main()
{
    if (useTexture)
    {
        fragColor = texture(texSampler, vTexCoord);
    }
    else
    {
        fragColor = color;
    }
}