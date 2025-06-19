#include "Coin.h"
#include "stb_image.h"
#include <iostream>

Coin::Coin(const std::string& textureFile, int tileWidth, int tileHeight, glm::vec2 pos)
    : position(pos)
{
    float vertices[] = {
        -tileWidth / 2.0f, -tileHeight / 2.0f, 0.0f, 0.0f,
         tileWidth / 2.0f, -tileHeight / 2.0f, 1.0f, 0.0f,
         tileWidth / 2.0f,  tileHeight / 2.0f, 1.0f, 1.0f,
        -tileWidth / 2.0f,  tileHeight / 2.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    loadTexture(textureFile);
}

void Coin::loadTexture(const std::string& filename) {
    int w, h, channels;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load coin texture: " << filename << std::endl;
        return;
    }
    textureWidth = w;
    textureHeight = h;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Coin::draw(GLuint shaderProgram, float tileWidth, float tileHeight) {
    if (collected) return; // não desenha se já foi coletada

    float tileX = position.x;
    float tileY = position.y;
    float px = (tileX - tileY) * (tileWidth / 2.0f);
    float py = (tileX + tileY) * (tileHeight / 2.0f) - 8.0f; // deslocamento opcional

    GLint loc = glGetUniformLocation(shaderProgram, "playerWorldPos");
    glUniform2f(loc, px, py);

    GLint debugColorLoc = glGetUniformLocation(shaderProgram, "debugSolidColor");
    glUniform4f(debugColorLoc, 0.0f, 0.0f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
