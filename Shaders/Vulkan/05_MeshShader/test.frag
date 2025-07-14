#version 460

layout(location = 0) in vec3 color;
layout(location = 0) out vec4 outColor;

struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

layout(set = 0, binding = 0) uniform SceneData {
    mat4 u_ViewProjectionMatrix;
    PointLight light;
    vec3 viewPos;
} scene;

void main() {
    outColor = vec4(color, 1.0);
}
