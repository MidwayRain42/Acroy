#version 460 core

layout (location = 0) in vec3 aPosition;

layout(std140, binding = 0) uniform PerFrame
{
    mat4 u_view;
    mat4 u_proj;
    float u_time;
};

layout(location = 0) out vec3 vTexCoords;

void main()
{
    vTexCoords = aPosition;

    mat4 viewNoTranslation = mat4(mat3(u_view));

    vec4 pos = u_proj * viewNoTranslation * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}