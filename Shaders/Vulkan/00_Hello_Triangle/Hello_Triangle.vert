#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 innormal;
layout(location = 2) in vec2 intexCord;

layout(set = 0, binding = 0) uniform SceneData {
    mat4 u_ViewProjectionMatrix;
} scene;



layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
} pushConstants;

layout(set = 1, binding = 0) uniform Color {
   vec4 color[3];
};

layout(location = 0) out vec4 outcolor;

void main() {
    outcolor =color[gl_VertexIndex] ;
    gl_Position =scene.u_ViewProjectionMatrix*pushConstants.modelMatrix*vec4(inPosition, 1.0);
}
