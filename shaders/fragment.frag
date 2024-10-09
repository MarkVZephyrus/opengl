#version 460 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform float offset;
uniform sampler2D texture1;
// look up "texture units". It auto assign the texture to this uniform.
// also used to pass multiple textures to the same shader.

void main() {
    vec4 color = vec4(vertexColor.x + abs(sin(offset) + 0.5f),
            vertexColor.y + abs(sin(offset) + 0.25f),
            vertexColor.z + abs(sin(offset)), 1.0);
    FragColor = texture(texture1, TexCoord) * color;
}
