#version 450 core

uniform mat4 u_transform;

struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

layout(std140,  binding = 0) uniform SceneData {
    mat4 u_ViewProjectionMatrix;
    PointLight light;
    vec3 viewPos;
}scene;


in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
	//归一化向量
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