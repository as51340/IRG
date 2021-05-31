#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec3 normal; //normal
out vec2 textureCoords; //texture coordinates
out vec4 fragPosLightSpace; //fragment position in light space

void main()
{
    textureCoords = in_textureCoords; //texture coordinates
    normal = in_normal; //normals

    fragPosLightSpace = lightSpaceMatrix * model * vec4(aPos, 1.0); //light fragment position so we can perform correct calculation in fragment shader

    gl_Position = projection * view * model * vec4(aPos, 1.0); //uvijek se mora definirati 
}