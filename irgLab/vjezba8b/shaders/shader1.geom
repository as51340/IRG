#version 330 core //this is geometry shader for scene system, use first algorithm

layout (triangles) in; // get three vertices
layout (triangle_strip, max_vertices = 3) out; // koji output?


out vec3 color; //send result to the fragment shader

void main()
{
	vec3 center;
	int i;

	vec3 v1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	v1 = normalize(v1);
	vec3 v2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	v2 = normalize(v2);
	vec3 poly_normal = cross(v1, v2); //polygon normal
	poly_normal = normalize(poly_normal);

	center.x = (gl_in[0].gl_Position.x + gl_in[1].gl_Position.x + gl_in[2].gl_Position.x) / 3;
	center.y = (gl_in[0].gl_Position.y + gl_in[1].gl_Position.y + gl_in[2].gl_Position.y) / 3;
	center.z = (gl_in[0].gl_Position.z + gl_in[1].gl_Position.z+ gl_in[2].gl_Position.z) / 3;



	color = vec3(ir, ig, ib); //jedan poligon jedna boja, trebalo bi bit okej

	for (i=0; i < gl_in.length(); i++) { //length is 3	
		gl_Position = projection * view * model * gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
}