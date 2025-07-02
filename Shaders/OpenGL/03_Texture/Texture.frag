#version 450 core

uniform mat4 u_transform;
uniform sampler2D TexSampler;

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

in vec2 Texture;
out vec4 FragColor;


void main()
{

    FragColor = texture(TexSampler,Texture);
    //FragColor=vec4(1,1,1,1);
}