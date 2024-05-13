#include <cstring>

const char *fragment = 
R"(#version 410 core

layout (location = 0) in vec2 FragTexCoord;
layout (location = 0) out vec4 FragColor;

uniform sampler2D textureSampler;

void main() {
    FragColor = texture(textureSampler, FragTexCoord);
})";
unsigned int fragment_len = strlen(fragment);

const char *fragment_es = 
R"(#version 100

precision mediump float;
varying vec2 FragTexCoord;

uniform sampler2D textureSampler;

void main() {
    gl_FragColor = texture2D(textureSampler, FragTexCoord);
})";
unsigned int fragment_es_len = strlen(fragment_es);

const char *vertex = 
R"(#version 410 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

layout (location = 0) out vec2 FragTexCoord;

void main() {
    gl_Position = vec4(position,0.0,1.0);
    FragTexCoord = texCoord;
})";
unsigned int vertex_len = strlen(vertex);

const char *vertex_es = 
R"(#version 100

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 FragTexCoord;

void main() {
    gl_Position = vec4(position,0.0,1.0);
    FragTexCoord = texCoord;
})";
unsigned int vertex_es_len = strlen(vertex_es);
