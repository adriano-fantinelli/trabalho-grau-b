#pragma once
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Player {
public:
    Player(const std::string& textureFile, int tileWidth, int tileHeight, int initialSpriteIndex = 0);
    void draw(GLuint shaderProgram, float tileWidth, float tileHeight);
    void move(int dx, int dy, const std::vector<int>& tiles, int mapWidth, int mapHeight);
    void setSpriteIndex(int index);

    glm::vec2 position;

private:
    GLuint VAO, VBO, EBO, textureID;
    int tileWidth, tileHeight;
    int textureWidth, textureHeight;
    int spriteIndex;

    void loadTexture(const std::string& filename);
    void updateSpriteUV();
};
