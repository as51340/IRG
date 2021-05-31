#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
out vec3 color;

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


void main()
{

	vec3 center_light = light_position - aPos;

    vec3 k = dot(normalize(center_light), normal) * normal * length(center_light);
    vec3 p = k - center_light;
    vec3 r = center_light + 2*p;


	center_light = normalize(center_light);

    vec3 center_eye = eye - aPos;
	center_eye = normalize(center_eye);

    float m1 = dot(center_light, normal);
    if(m1 < 0) 
    {
        m1 = 0.0;
    }

    //vec3 h = (center_light + center_eye) / 2; //for reflection purposes

    //h = normalize(h);
    r = normalize(r);

    float m2 = dot(center_eye, r);
    if(m2 < 0) {
		m2 = 0.0;
	}

    float ir = ia.x*amb.x + ii.x * (diff.x * m1 + ref.x*pow(m2, 10));
    float ig = ia.y*amb.y + ii.y * (diff.y * m1 + ref.y*pow(m2, 10));
    float ib = ia.z*amb.z + ii.z * (diff.z * m1 + ref.z*pow(m2, 10));

    color = vec3(ir, ig, ib);

    gl_Position = projection * view * model * vec4(aPos, 1.0); //uvijek se mora definirati 
}