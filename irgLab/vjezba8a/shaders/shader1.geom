#version 330 core //this is geometry shader for scene system, use first algorithm

layout (triangles) in; // get three vertices
layout (triangle_strip, max_vertices = 3) out; // koji output?

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 eye;
uniform vec3 light_position;
uniform vec3 amb;
uniform vec3 diff;
uniform vec3 ref;
uniform vec3 ii; //intensity i
uniform vec3 ia; //intensity ambient

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


	vec3 center_light = light_position - center;
	center_light = normalize(center_light);
    vec3 center_eye = eye - center;
	center_eye = normalize(center_eye);

    float m1 = dot(center_light, poly_normal);
    if(m1 < 0) 
    {
        m1 = 0.0;
    }

    vec3 h = (center_light + center_eye) / 2; //for reflection purposes
	h = normalize(h);
    float m2 = dot(center_eye, h);
    if(m2 < 0) {
		m2 = 0.0;
	}

    float ir = ia.x*amb.x + ii.x * (diff.x * m1 + ref.x*pow(m2, 10));
    float ig = ia.y*amb.y + ii.y * (diff.y * m1 + ref.y*pow(m2, 10));
    float ib = ia.z*amb.z + ii.z * (diff.z * m1 + ref.z*pow(m2, 10));

	color = vec3(ir, ig, ib); //jedan poligon jedna boja, trebalo bi bit okej

	for (i=0; i < gl_in.length(); i++) { //length is 3	
		gl_Position = projection * view * model * gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
}