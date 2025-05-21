#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
void main() {
    gl_Position = vec4(aPos.x / 400.0, -aPos.y / 300.0, 0.0, 1.0);
    TexCoord = aTexCoord;
}
