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

layout(push_constant) uniform PushConstants {
    mat4 u_transform;
} pushConstants;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;

layout(location = 0) out vec4 FragColor;

void main() {
    // 归一化向量
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(scene.light.position.xyz - FragPos);
    vec3 viewDir = normalize(scene.viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); // Blinn-Phong 半角向量

    // 环境光
    vec3 ambient = scene.light.ambient.xyz ;

    // 漫反射
    float diff = max(dot(norm, halfwayDir), 0.0);
    vec3 diffuse = scene.light.diffuse.xyz * (diff );

    // 高光（Blinn-Phong优化）
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 2);
    vec3 specular = scene.light.specular.xyz * (spec );

    // 合并结果
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
