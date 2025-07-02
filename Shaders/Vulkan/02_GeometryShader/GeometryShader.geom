// ===================== 几何着色器 (geom) =====================
#version 450


layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

layout(location = 0) in vec3 vPosition[];

layout(location = 0) out vec4 fragColor;

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

layout(set = 1, binding = 0) uniform Length {
    float normalLength;
};

void main() {
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

    gl_Position = scene.u_ViewProjectionMatrix * pushConstants.u_transform * vec4(center, 1.0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0); // 起点颜色
    EmitVertex();

    gl_Position = scene.u_ViewProjectionMatrix * pushConstants.u_transform * vec4(end, 1.0);
    fragColor = vec4(0.0, 1.0, 0.0, 1.0); // 终点颜色
    EmitVertex();

    EndPrimitive();
}
