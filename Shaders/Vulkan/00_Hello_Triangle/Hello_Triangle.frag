#version 450

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform SceneData {
    mat4 u_ViewProjectionMatrix;
} scene;

layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
} pushConstants;

layout(location = 0) in vec4 incolor;

void main() {
    outColor = incolor;
}
