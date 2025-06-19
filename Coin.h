#pragma once
#include <glm/glm.hpp>
#include "glad/glad.h"
#include <string>

class Coin {
public:
    Coin(const std::string& textureFile, int tileWidth, int tileHeight, glm::vec2 pos);
    void draw(GLuint shaderProgram, float tileWidth, float tileHeight);
    glm::vec2 position;
    bool isCollected() const { return collected; }
    void collect() { collected = true; }

private:
    GLuint VAO, VBO, EBO, textureID;
    int textureWidth, textureHeight;
    bool collected = false;
    void loadTexture(const std::string& filename);
};
