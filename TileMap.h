#pragma once
#include <vector>
#include <string>
#include "glad/glad.h"
#include <glm/glm.hpp>

class TileMap {
public:
    bool load(const std::string& mapFile, const std::string& tilesetFile, int tileWidth, int tileHeight, int tilesetColumns);
    void draw();
    int getWidth() const;
    int getHeight() const;
    const std::vector<int>& getTiles() const { return tiles; }
private:
    std::vector<int> tiles;  // Mude para 1D vector
    int width, height;
    GLuint VAO, VBO, EBO, textureID;
    int mapWidth, mapHeight;

    void loadTexture(const std::string& filename);
    void buildMesh(int tileWidth, int tileHeight, int tilesetCols);
};
