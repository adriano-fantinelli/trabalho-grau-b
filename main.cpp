#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "TileMap.h"
#include <fstream>
#include "Player.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Funções utilitárias para carregar shaders
GLuint LoadShader(const char* vertexPath, const char* fragmentPath) {
    // Carregue os arquivos vertex.glsl e fragment.glsl para strings
    std::ifstream vFile(vertexPath), fFile(fragmentPath);
    std::string vSrc((std::istreambuf_iterator<char>(vFile)), std::istreambuf_iterator<char>());
    std::string fSrc((std::istreambuf_iterator<char>(fFile)), std::istreambuf_iterator<char>());
    const char* vShaderCode = vSrc.c_str();
    const char* fShaderCode = fSrc.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Tilemap Isométrico", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint shaderProgram = LoadShader("vertex.glsl", "fragment.glsl");
    glUseProgram(shaderProgram);

    TileMap map;
    map.load("assets/map.txt", "assets/tilesetIso.png", 32, 32, 11);

    // Calcule o centro do mapa
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();
    float tileWidth = 32.0f;
    float tileHeight = 32.0f;

    // Centralização correta para isométrico diamond:
    float centerX = ((mapWidth - 1) - (mapHeight - 1)) * (tileWidth / 2.0f) / 2.0f;
    float centerY = ((mapWidth - 1) + (mapHeight - 1)) * (tileHeight / 2.0f) / 2.0f;

    int centerLoc = glGetUniformLocation(shaderProgram, "center");

    Player player("assets/critter_stag_SE_idle.png", 32, 41);
    player.position = glm::vec2((mapWidth - 1) / 2.0f, (mapHeight - 1) / 2.0f);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Envie o centro para o shader
        glUniform2f(centerLoc, centerX, centerY);

        // Controle do teclado
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player.move(0, -1, map.getTiles(), mapWidth, mapHeight); // Norte
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player.move(0, 1, map.getTiles(), mapWidth, mapHeight);  // Sul
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player.move(-1, 0, map.getTiles(), mapWidth, mapHeight); // Oeste
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player.move(1, 0, map.getTiles(), mapWidth, mapHeight);  // Leste
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) player.move(-1, -1, map.getTiles(), mapWidth, mapHeight); // Noroeste
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) player.move(1, -1, map.getTiles(), mapWidth, mapHeight);  // Nordeste
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) player.move(-1, 1, map.getTiles(), mapWidth, mapHeight); // Sudoeste
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) player.move(1, 1, map.getTiles(), mapWidth, mapHeight);  // Sudeste

        // Desenhe o mapa e o personagem
        map.draw();
        player.draw(shaderProgram, tileWidth, tileHeight);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
