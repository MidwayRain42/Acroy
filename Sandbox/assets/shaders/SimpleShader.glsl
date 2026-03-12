#type vertex
#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec2 v_texCoord;

void main()
{
    gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
    v_texCoord = a_texCoord;
}


#type fragment
#version 460 core

out vec4 FragColor;

in vec2 v_texCoord;

uniform sampler2D u_texture;
uniform float u_textureScale;

void main()
{
    FragColor = texture(u_texture, v_texCoord * u_textureScale);
}