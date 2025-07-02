#version 450
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
}scene;

layout(set = 1, binding = 0) uniform  sampler2D TexSampler;

layout(push_constant) uniform PushConstants {
    mat4 u_transform;
} pushConstants;

layout(location = 0) in vec2 Texture;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(TexSampler,Texture);
}
