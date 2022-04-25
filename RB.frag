/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

texturedlit.frag
Fragment shader.
Uses a fixed directional light grey light with only diffuse and ambient lighting.
You'll learn a lot about lighting in graphics - this shader just gives us a simple
way to make sure that everything looks right with our normals, etc.
*/

#version 420 core

out vec4 FragColor;
  
in vec2 outUV;

layout(binding = 0)uniform sampler2D albedo;

uniform float texX;
uniform float texY;
uniform int shouldBuffer;
uniform float filmGrainStrength = 0;
uniform int blurStr = 0;
uniform float passedTime = 0;
uniform vec2 screenRes;


void main()
{ 

vec4 passingColour = texture(albedo, outUV);
if(shouldBuffer == 1){



float x = (outUV.x + 4.0 ) * (outUV.y + 4.0 ) * (passedTime * 10.0);//https://www.shadertoy.com/view/4sXSWs
vec4 grain = vec4(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01)-0.005) * filmGrainStrength;
grain = 1.0 - grain;
passingColour = passingColour * grain ;
}


FragColor = passingColour;
}