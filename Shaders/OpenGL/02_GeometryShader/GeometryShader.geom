#version 450 core

layout(triangles) in;                 // 输入是三角形
layout(line_strip, max_vertices = 2) out; // 输出线段，两个顶点构成一条线

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

layout(std140, binding = 1) uniform Length {
    float normalLength;
};
in vec3 vPosition[];
out vec4 fragColor;

void main()
{
    // 三角形三个顶点
    vec3 p0 = vPosition[0];
    vec3 p1 = vPosition[1];
    vec3 p2 = vPosition[2];

    // 计算面中心（顶点平均）
    vec3 center = (p0 + p1 + p2) / 3.0;

    // 计算面法向量（叉积）
    vec3 edge1 = p1 - p0;
    vec3 edge2 = p2 - p0;
    vec3 faceNormal = normalize(cross(edge1, edge2));

    // 可视化线段：center -> center + normal * length
    vec3 end = center + faceNormal * normalLength;

    gl_Position = scene.u_ViewProjectionMatrix * u_transform * vec4(center, 1.0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0); // 起点颜色
    EmitVertex();

    gl_Position = scene.u_ViewProjectionMatrix * u_transform * vec4(end, 1.0);
    fragColor = vec4(0.0, 1.0, 0.0, 1.0); // 终点颜色
    EmitVertex();

    EndPrimitive();

}