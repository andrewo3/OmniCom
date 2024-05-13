#if defined(GL_ES)
    #version 100
#elif defined(GL_CORE)
    #version 410 core
#endif

layout (location = 0) in vec2 FragTexCoord;
layout (location = 0) out vec4 FragColor;

uniform sampler2D textureSampler;

void main() {
    FragColor = texture(textureSampler, FragTexCoord);
}