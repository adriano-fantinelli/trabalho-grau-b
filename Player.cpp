#include "Player.h"
#include "stb_image.h"
#include <iostream>
#include <glm/glm.hpp>

Player::Player(const std::string& textureFile, int tileWidth, int tileHeight)
    : tileWidth(tileWidth), tileHeight(tileHeight), position(0, 0) {
    loadTexture(textureFile);

    float u0 = 0.0f;
    float v0 = 0.0f;
    float u1 = tileWidth / (float)textureWidth;
    float v1 = tileHeight / (float)textureHeight;

    float yOffset = (tileHeight / 2.0f) - tileHeight + 5.0f;

    float vertices[] = {
        -tileWidth / 2.0f, -tileHeight / 2.0f + yOffset, u0, v0, 
         tileWidth / 2.0f, -tileHeight / 2.0f + yOffset, u1, v0, 
         tileWidth / 2.0f,  tileHeight / 2.0f + yOffset, u1, v1, 
        -tileWidth / 2.0f,  tileHeight / 2.0f + yOffset, u0, v1 
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
}

void Player::loadTexture(const std::string& filename) {
    int w, h, channels;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        std::cerr << "STBI Error: " << stbi_failure_reason() << std::endl;
        return;
    }
    std::cout << "Player texture loaded: " << filename << " (" << w << "x" << h << ")" << std::endl;
    textureWidth = w;
    textureHeight = h;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Player::draw(GLuint shaderProgram, float tileWidth, float tileHeight) {
    float tileX = position.x;
    float tileY = position.y;
    float px = (tileX - tileY) * (tileWidth / 2.0f);
    float py = (tileX + tileY) * (tileHeight / 2.0f);

    GLint loc = glGetUniformLocation(shaderProgram, "playerWorldPos");
    glUniform2f(loc, px, py);

    GLint debugColorLoc = glGetUniformLocation(shaderProgram, "debugSolidColor");
    glUniform4f(debugColorLoc, 0.0f, 0.0f, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Player::move(int dx, int dy, const std::vector<int>& tiles, int mapWidth, int mapHeight) {
    if (dx == 0 && dy == 0) return;

    glm::vec2 newPosition = position + glm::vec2(dx, dy);

    if (newPosition.x < 0 || newPosition.y < 0 || newPosition.x >= mapWidth || newPosition.y >= mapHeight)
        return;

    int tileIndex = (int)newPosition.y * mapWidth + (int)newPosition.x;
    int tileValue = tiles[tileIndex];
    if (tileValue < 0 || tileValue > 6)
        return;

    position = newPosition;
}
