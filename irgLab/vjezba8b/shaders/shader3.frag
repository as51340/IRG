#version 330 core
out vec4 FragColor;  //izlaz iz sjencara fragmenata je boja tipa vec4(red, green, blue, alpha)

in vec3 in_color; //ulaz u sjencar fragmenata je izlaz iz sjencara vrhova u obliku vec3(red, green, blue)

void main()
{
    FragColor = vec4(in_color, 0.0);  //samo zapisemo boju
} 