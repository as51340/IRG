#version 330 core

in vec3 color;
out vec4 FragColor;  //izlaz iz sjencara fragmenata je boja tipa vec4(red, green, blue, alpha)

void main()
{
    FragColor = vec4(color, 1.0);  //samo zapisemo boju
} 