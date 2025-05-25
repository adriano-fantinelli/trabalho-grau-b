#include "Player.h"
#include "stb_image.h"
#include <iostream>

Player::Player(const std::string& textureFile, int tileWidth, int tileHeight)
    : tileWidth(tileWidth), tileHeight(tileHeight), position(0, 0) {
    loadTexture(textureFile);

    // Configuração do VAO/VBO para o sprite do personagem
    float vertices[] = {
        // Posições      // Coordenadas de textura
        -tileWidth / 2, -tileHeight / 2, 0.0f, 1.0f,
         tileWidth / 2, -tileHeight / 2, 1.0f, 1.0f,
         tileWidth / 2,  tileHeight / 2, 1.0f, 0.0f,
        -tileWidth / 2,  tileHeight / 2, 0.0f, 0.0f,
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
        std::cerr << "STBI Error: " << stbi_failure_reason() << std::endl; // Adicione esta linha
        return;
    }

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

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Player::move(int dx, int dy, const std::vector<int>& tiles, int mapWidth, int mapHeight) {
    glm::vec2 newPosition = position + glm::vec2(dx, dy);

    // Verifique se a nova posição está dentro dos limites do mapa
    if (newPosition.x < 0 || newPosition.y < 0 || newPosition.x >= mapWidth || newPosition.y >= mapHeight)
        return;

    // Verifique se o tile é caminhável
    int tileIndex = newPosition.y * mapWidth + newPosition.x;
    if (tiles[tileIndex] != 0) // Suponha que 0 é um tile não caminhável
        return;

    position = newPosition;
}
