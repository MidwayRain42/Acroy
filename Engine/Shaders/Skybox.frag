#version 460 core

layout(location = 0) out vec4 fragColor;
layout(location = 0) in vec3 vTexCoords;

layout(set = 0, binding = 0) uniform samplerCube skybox;

void main()
{
    fragColor = texture(skybox, vTexCoords);
}