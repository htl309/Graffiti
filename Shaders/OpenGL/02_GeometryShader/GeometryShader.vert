#version 450 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 innormal;
layout(location = 2) in vec2 intexCord;

uniform mat4 u_transform;

struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

layout(std140, binding = 0) uniform SceneData {
    mat4 u_ViewProjectionMatrix;
	PointLight light;
    vec3 viewPos;
}scene;

out vec3 vPosition;

void main()
{
    vPosition=inPosition;
    gl_Position =scene.u_ViewProjectionMatrix*u_transform*vec4(inPosition, 1.0);
}