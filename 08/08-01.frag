#version 330 core

in vec2 TexCoord;
out vec4 frag_color;

uniform sampler2D texSampler0;
uniform sampler2D texSampler1;

void main() {
    frag_color = mix(texture(texSampler0, TexCoord), texture(texSampler1, TexCoord), 0.2);
}
