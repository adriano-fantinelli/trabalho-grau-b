#include "TileMap.h"
#include <fstream>
#include <sstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool TileMap::load(const std::string& mapFile, const std::string& tilesetFile, int tileWidth, int tileHeight, int tilesetCols) {
    std::ifstream in(mapFile);
    if (!in) return false;

    in >> mapWidth >> mapHeight;
    tiles.resize(mapWidth * mapHeight);

    for (int i = 0; i < mapWidth * mapHeight; ++i) {
        in >> tiles[i];
    }

    loadTexture(tilesetFile);
    buildMesh(tileWidth, tileHeight, tilesetCols);
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

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int tileID = tiles[y * mapWidth + x];
            int tu = tileID % tilesetCols;
            int tv = tileID / tilesetCols;

            float posX = (x - y) * tileWidth / 2.0f;
            float posY = (x + y) * tileHeight / 2.0f;

            float u0 = tu * (1.0f / tilesetCols);
            float v0 = tv * (1.0f / tilesetCols); // aprox. (ajustar pela altura real)
            float u1 = u0 + 1.0f / tilesetCols;
            float v1 = v0 + 1.0f / tilesetCols;

            vertices.insert(vertices.end(), {
                posX, posY, u0, v0,
                posX + tileWidth / 2, posY + tileHeight / 2, u1, v0,
                posX, posY + tileHeight, u1, v1,
                posX - tileWidth / 2, posY + tileHeight / 2, u0, v1
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
