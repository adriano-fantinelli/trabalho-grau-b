#pragma once
#include <vector>
#include <string>
#include <../glad/glad.h>
#include <glm/glm.hpp>

class TileMap {
public:
    bool load(const std::string& mapFile, const std::string& tilesetFile, int tileWidth, int tileHeight, int tilesetCols);
    void draw();

private:
    GLuint VAO, VBO, EBO, textureID;
    int mapWidth, mapHeight;
    std::vector<int> tiles;

    void loadTexture(const std::string& filename);
    void buildMesh(int tileWidth, int tileHeight, int tilesetCols);
};
