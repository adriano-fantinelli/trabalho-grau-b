#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform vec2 center;
uniform vec2 playerWorldPos;

void main() {
    vec2 pos = aPos + playerWorldPos;
    gl_Position = vec4((pos.x - center.x) / 330.0, -(pos.y - center.y) / 247.5, 0.0, 1.0);
    TexCoord = aTexCoord;
}
