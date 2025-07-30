#version 460

in VertexOut
{
	vec3 color;
} fragColor;

out vec4 outColor;

void main() {
  outColor = vec4(fragColor.color,1);
}
