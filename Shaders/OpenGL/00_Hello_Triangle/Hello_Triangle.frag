#version 450 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 innormal;
layout(location = 2) in vec2 intexCord;

in vec4 u_color;
out vec4 fragColor;

void main()
{
	fragColor = u_color;
	
}