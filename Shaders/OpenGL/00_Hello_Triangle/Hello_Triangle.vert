#version 450 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 innormal;
layout(location = 2) in vec2 intexCord;

uniform mat4 u_transform;

layout(std140, binding = 0) uniform SceneData {
    mat4 u_ViewProjectionMatrix;
}scene;
 layout(std140, binding = 1) uniform color {
      vec4 colors[3];
 };

out vec4 u_color;
void main()
{

	u_color =colors[gl_VertexID];
	gl_Position =scene.u_ViewProjectionMatrix*u_transform*vec4(inPosition, 1.0);	
}