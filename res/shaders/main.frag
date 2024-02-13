#version 450 core
layout (location = 0) in vec2 FragTexCoord;
layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform sampler2D textureSampler;

void main() {
    FragColor = texture(textureSampler, FragTexCoord);
}