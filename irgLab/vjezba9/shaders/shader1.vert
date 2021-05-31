#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec2 textureCoords;

void main()
{
    textureCoords = in_textureCoords;
    normal = in_normal;
    gl_Position = projection * view * model * vec4(aPos, 1.0); //uvijek se mora definirati 
}