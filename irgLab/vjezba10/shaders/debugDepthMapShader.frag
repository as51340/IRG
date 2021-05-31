#version 330 core
out vec4 FragColor;
  
in vec2 textureCoords;

uniform sampler2D ourTexture;

void main()
{             
    float depthValue = texture(ourTexture, textureCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}  