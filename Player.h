#pragma once
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Player {
public:
    Player(const std::string& textureFile, int tileWidth, int tileHeight);
    void draw(GLuint shaderProgram, float tileWidth, float tileHeight);
    void move(int dx, int dy, const std::vector<int>& tiles, int mapWidth, int mapHeight);
    glm::vec2 position; // Posição do personagem no mapa (em tiles)

private:
    GLuint VAO, VBO, EBO, textureID;
    int tileWidth, tileHeight;

    void loadTexture(const std::string& filename);
};