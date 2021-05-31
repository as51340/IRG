#version 330 core //this is geometry shader for scene system, use first algorithm

layout (triangles) in; //sjenèar geometrije se izvodi nakon sjenèara vrhova, a vertex shader šalje samo vrhove
uniform vec3 color; //primi boju
out vec3 in_color; //output color?
layout (line_strip, max_vertices = 3) out; //3 is okay because we are accepting one triangle at a time, ne slati normale sjenèaru geometrije

void main()
{
	in_color = color; //send color
	int i;

	vec4 v1 = gl_in[1].gl_Position.xyzw - gl_in[0].gl_Position.xyzw;
	float dot = dot(v1, gl_in[2].gl_Position.xyzw);
	if(dot <= 0.0) { //CW
		for (i=0; i < gl_in.length(); i++) { //length is 3	
			gl_Position = gl_in[i].gl_Position;			
			EmitVertex();
		}
	}
	EndPrimitive();
}