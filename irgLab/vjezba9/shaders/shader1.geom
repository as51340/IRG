#version 330 core //this is geometry shader for scene system, use first algorithm

layout (triangles) in; // get three vertices
layout (triangle_strip, max_vertices = 3) out;

void main()
{
	int i;
	for (i=0; i < gl_in.length(); i++) { //length is 3	
		gl_Position = projection * view * model * gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
}