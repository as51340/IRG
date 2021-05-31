#version 330 core //this is geometry shader for scene system, use first algorithm

layout (triangles) in; //sjen�ar geometrije se izvodi nakon sjen�ara vrhova, a vertex shader �alje samo vrhove
uniform vec3 color; //primi boju
out vec3 in_color; //output color?
layout (line_strip, max_vertices = 3) out; //3 is okay because we are accepting one triangle at a time, ne slati normale sjen�aru geometrije

void main()
{
	in_color = color; //send color
	int i;

	vec3 v1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 v2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 v3 = cross(v1, v2);
	vec3 v4 = vec3(0,0,-1);
	float d = dot(v3, v4);
	if(d <= 0.0) { //CW
		for (i=0; i < gl_in.length(); i++) { //length is 2
			gl_Position = gl_in[i].gl_Position;			
			EmitVertex();
		}
	}
	EndPrimitive();
}