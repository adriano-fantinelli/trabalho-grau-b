#include "TileMap.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "stb_image.h"
#include <algorithm>

bool TileMap::load(const std::string& mapFile, const std::string& tilesetFile, int tileWidth, int tileHeight, int tileCount) {
    std::ifstream in(mapFile);
    if (!in) return false;

    in >> mapWidth >> mapHeight;
    tiles.resize(mapWidth * mapHeight);

    for (int i = 0; i < mapWidth * mapHeight; ++i) {
        in >> tiles[i];
    }

    loadTexture(tilesetFile);
    buildMesh(tileWidth, tileHeight, tileCount);
    return true;
}

void TileMap::loadTexture(const std::string& filename) {
    int w, h, channels;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &channels, 4);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void TileMap::buildMesh(int tileWidth, int tileHeight, int tilesetCols) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    int index = 0;

    int tilesetWidth, tilesetHeight, channels;
    stbi_info("assets/tilesetIso.png", &tilesetWidth, &tilesetHeight, &channels);

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int tileID = tiles[y * mapWidth + x];
            int tu = tileID % tilesetCols;
            int tv = tileID / tilesetCols;

            float centerX = (x - y) * (tileWidth / 2.0f);
            float centerY = (x + y) * (tileHeight / 2.0f);

            float u0 = tu * (tileWidth / (float)tilesetWidth);
            float v0 = tv * (tileHeight / (float)tilesetHeight);
            float u1 = u0 + (tileWidth / (float)tilesetWidth);
            float v1 = v0 + (tileHeight / (float)tilesetHeight);

            vertices.insert(vertices.end(), {
                centerX, centerY - tileHeight / 2, (u0 + u1) / 2, v0,           // topo
                centerX + tileWidth / 2, centerY, u1, (v0 + v1) / 2,            // direita
                centerX, centerY + tileHeight / 2, (u0 + u1) / 2, v1,           // baixo
                centerX - tileWidth / 2, centerY, u0, (v0 + v1) / 2             // esquerda
            });

            indices.insert(indices.end(), {
                index, index + 1, index + 2,
                index, index + 2, index + 3
            });

            index += 4;
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // tex
    glEnableVertexAttribArray(1);
}

void TileMap::draw() {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mapWidth * mapHeight * 6, GL_UNSIGNED_INT, 0);
}

int TileMap::getWidth() const {
    return mapWidth;
}

int TileMap::getHeight() const {
    return mapHeight;
}
