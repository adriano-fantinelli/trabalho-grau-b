#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "TileMap.h"
#include <fstream>
#include "Player.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLuint LoadShader(const char *vertexPath, const char *fragmentPath)
{
    std::ifstream vFile(vertexPath), fFile(fragmentPath);
    std::string vSrc((std::istreambuf_iterator<char>(vFile)), std::istreambuf_iterator<char>());
    std::string fSrc((std::istreambuf_iterator<char>(fFile)), std::istreambuf_iterator<char>());
    const char *vShaderCode = vSrc.c_str();
    const char *fShaderCode = fSrc.c_str();

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

int main()
{
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Tilemap Isométrico", NULL, NULL);
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

    float centerX = ((mapWidth - 1) - (mapHeight - 1)) * (tileWidth / 2.0f) / 2.0f;
    float centerY = ((mapWidth - 1) + (mapHeight - 1)) * (tileHeight / 2.0f) / 2.0f;

    int centerLoc = glGetUniformLocation(shaderProgram, "center");

    Player player("assets/critter_stag_SE_idle.png", 32, 41); // imagem e tamanho do boneco
    player.position = glm::vec2((mapWidth - 1) / 2.0f, (mapHeight - 1) / 2.0f); // inicializa o personagem no meio do mapa

    std::vector<int> tiles = map.getTiles();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        int dx = 0, dy = 0;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            dx -= 1;
            dy -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            dx += 1;
            dy += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            dx -= 1;
            dy += 1;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            dx += 1;
            dy -= 1;
        } 
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            dx -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            dy -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            dy += 1;
        } 
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            dx += 1;
        }
        if (dx != 0 || dy != 0)
        {
            player.move(dx, dy, tiles, mapWidth, mapHeight);
            glfwWaitEventsTimeout(0.15);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glUniform2f(centerLoc, centerX, centerY);
        GLint playerWorldPosLoc = glGetUniformLocation(shaderProgram, "playerWorldPos");
        glUniform2f(playerWorldPosLoc, 0.0f, 0.0f);
        map.draw();
        float tileX = player.position.x;
        float tileY = player.position.y;
        float px = (tileX - tileY) * (tileWidth / 2.0f);
        float py = (tileX + tileY) * (tileHeight / 2.0f);
        glUniform2f(playerWorldPosLoc, px, py);
        player.draw(shaderProgram, tileWidth, tileHeight);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
