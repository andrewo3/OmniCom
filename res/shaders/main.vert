#if defined(GL_ES)
    #version 100
#elif defined(GL_CORE)
    #version 410 core
#endif

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

layout (location = 0) out vec2 FragTexCoord;

void main() {
    gl_Position = vec4(position,0.0,1.0);
    FragTexCoord = texCoord;
}