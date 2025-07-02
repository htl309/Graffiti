#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 innormal;
layout(location = 2) in vec2 intexCord;

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

layout(push_constant) uniform PushConstants {
    mat4 u_transform;
} pushConstants;


layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec3 Normal;


void main() {

    FragPos=(pushConstants.u_transform*vec4(inPosition, 1.0)).xyz;
    Normal=innormal;
    gl_Position =scene.u_ViewProjectionMatrix*vec4(FragPos, 1.0);
}
