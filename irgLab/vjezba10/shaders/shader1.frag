#version 330 core

in vec3 normal;
in vec2 textureCoords; //out from vertex shader
in vec4 fragPosLightSpace; //out from vertex shader

uniform vec3 eye;
uniform vec3 light_position;
uniform vec3 amb;
uniform vec3 diff;
uniform vec3 ref;
uniform vec3 ii; //intensity i
uniform vec3 ia; //intensity ambient

uniform sampler2D ourTexture; //texture
uniform sampler2D shadowMap; //shadow map

out vec4 FragColor;  //izlaz iz sjencara fragmenata je boja tipa vec4(red, green, blue, alpha)

float bias = 0.005;

bool shadowCalculation(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; // read about perspective division
    // transform to [0,1] range

    projCoords = projCoords * 0.5 + 0.5;
    /*float temp = projCoords.y;
    projCoords.y = projCoords.x;
    projCoords.x = temp;*/

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).b; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    bool shadow = currentDepth - bias > closestDepth  ? false : true;

    return shadow;
}

void main()
{
    
	vec3 center_light = light_position - normal; //

    vec3 k = dot(normalize(center_light), normal) * normal * length(center_light);
    vec3 p = k - center_light;
    vec3 r = center_light + 2*p;


	center_light = normalize(center_light);

    vec3 center_eye = eye - normal; //?
	center_eye = normalize(center_eye);

    float m1 = dot(center_light, normal);
    if(m1 < 0) 
    {
        m1 = 0.0;
    }

    r = normalize(r);

    float m2 = dot(center_eye, r);
    if(m2 < 0) {
		m2 = 0.0;
	}

    vec3 textureColor = texture(ourTexture, textureCoords).rgb;
    //vec3 shadowColor = texture(shadowMap, textureCoords).rgb * 0.5 + 0.5;
    //vec3 shadowColor = fragPosLightSpace.xyz * 0.5 + 0.5;
    bool shadowCalc = shadowCalculation(fragPosLightSpace);
    //FragColor = vec4(shadowColor, 1.0);
    //FragColor = vec4(shadowColor, 1.0);
    if(shadowCalc == false) {
        FragColor = vec4(vec3(ia.x*amb.x, ia.y*amb.y, ia.z*amb.z), 1.0);
        //FragColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
    } else {    
        float ir = ia.x*amb.x + ii.x * (textureColor.x * m1 + ref.x*pow(m2, 10));
        float ig = ia.y*amb.y + ii.y * (textureColor.y * m1 + ref.y*pow(m2, 10));
        float ib = ia.z*amb.z + ii.z * (textureColor.z * m1 + ref.z*pow(m2, 10));
        FragColor = vec4(vec3(ir, ig, ib), 1.0);  //samo zapisemo boju
    }
} 