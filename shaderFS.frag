#version 410

//layout(location = 1) in vec3 inColor;
//Lec5
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;


uniform sampler2D textureSampler;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform int mode;
uniform int mode2;
uniform int shouldAmbient;
uniform int shouldDiffuse;
uniform int shouldSpecular;

out vec4 frag_color;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main() { 

	//vec3 inColor = vec3(1,1,1);
	// Lec5 -- Ambient
	
	float ambientStrength = 0.6;
	vec3 ambient = ambientStrength * lightColor;// * inColor;
	
	// Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(lightPos - inPos);

	float d = max(dot(N, lightDir), 0.0); // we don't want negative diffuse
	vec3 diffuse = d * lightColor;

	//Attenuation
	float dist = length(lightPos - inPos);
	diffuse = diffuse / dist*dist;

	// Specular
	float specularStrength = 1.0;
	vec3 camDir = normalize(cameraPos - inPos);
	vec3 reflectedRay = reflect(-lightDir, N); // light direction to the point
	float spec = pow(max(dot(camDir, reflectedRay), 0.0), 512); // shininess coeficient
	vec3 specular = specularStrength * spec * lightColor;
	if(shouldAmbient == 1){//change the values of ambient, diffuse, and specular according to input
		ambient = vec3(0,0,0);
	}
	if(shouldDiffuse == 1){
		diffuse = vec3(0,0,0);
	}
	if(shouldSpecular == 1){
		specular = vec3(0,0,0);
	}
	vec4 lighting  = vec4(ambient + diffuse + specular, 1.0);
	if(mode == 1){
	lighting = vec4(1,1,1,1);
	}
	
	lighting.a = 1;
	vec4 textureCol = texture(textureSampler, inUV);
	if(mode2 == 1){
	textureCol = vec4(0.5,0.5,0.5,0.5);
	}
	//vec3 result = ambient;
	frag_color = textureCol * lighting;
}