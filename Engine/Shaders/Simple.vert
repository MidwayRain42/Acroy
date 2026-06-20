#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec2 vTexCoord;

layout(std140, binding = 0) uniform PerFrame
{
    mat4 u_view;
    mat4 u_proj;
    float u_time;
};

layout(std140, binding = 1) uniform PerObject
{
    mat4 u_model;
};

void main()
{
    gl_Position = u_proj * u_view * u_model * vec4(aPosition, 1.0);
    vColor      = aColor;
    vTexCoord   = aTexCoord;
}