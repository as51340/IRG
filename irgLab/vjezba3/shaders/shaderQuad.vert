#version 330 core
layout (location = 0) in vec3 aPos; 
//layout (location = 1) in vec3 aCol;

uniform vec3 u_color;  //uniformna varijabla je jednaka svim sjencarima pri jednom pozivu iscrtavanja

out vec3 color; //izlaz iz sjencara je boja koja se prilikom rasterizacije interpolira po cijelom trokutu

void main()
{
	color = u_color;
	//color = aCol;
    gl_Position = vec4(aPos, 1.0); //uvijek se mora definirati 
}