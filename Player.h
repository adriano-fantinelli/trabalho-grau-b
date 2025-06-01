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
    glm::vec2 position;

private:
    GLuint VAO, VBO, EBO, textureID;
    int tileWidth, tileHeight;
    int textureWidth, textureHeight;

    void loadTexture(const std::string& filename);
};