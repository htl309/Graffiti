#version 450


out vec4 outColor;

in VertexOut
{
	vec3 color;
} fragColor;


void main() {
    outColor = vec4(fragColor.color, 1.0);

}
