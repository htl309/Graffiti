#version 450


layout(location = 0) in vec4  inColor;
layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = inColor;
    //FragColor = vec4(1,1,0,1);
}
