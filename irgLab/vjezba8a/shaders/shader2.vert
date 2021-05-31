#version 330 core
layout (location = 0) in vec3 aPos;  //
//uniform vec3 u_color;  //uniformna varijabla je jednaka svim sjencarima pri jednom pozivu iscrtavanja
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform vec3 in_eye; //input eye

//out vec3 color; //izlaz iz sjencara je boja koja se prilikom rasterizacije interpolira po cijelom trokutu
//out vec3 eye; //send eye to geometry shader

void main()
{
	//color = u_color;
	//eye = in_eye;
    gl_Position = projection * view * model * vec4(aPos, 1.0); //uvijek se mora definirati 
}