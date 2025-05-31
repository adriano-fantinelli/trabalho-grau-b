#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
uniform vec4 debugSolidColor = vec4(0,0,0,0);
void main() {
    if (debugSolidColor.a > 0.0) {
        FragColor = debugSolidColor;
    } else {
        FragColor = texture(ourTexture, TexCoord);
    }
}
