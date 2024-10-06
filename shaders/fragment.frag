#version 460 core
out vec4 FragColor;
in vec3 vertexColor;
uniform float offset;
void main() {
    FragColor = vec4(vertexColor.x + abs(sin(offset) + 0.5f), vertexColor.y + abs(sin(offset) + 0.25f), vertexColor.z + abs(sin(offset)), 1.0);
}
