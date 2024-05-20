#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "shader.h"
#include "sprite_renderer.h"
#include "texture.h"

void processInput(GLFWwindow *window);
std::optional<std::string> loadTextFile(const std::string &filename);
Shader LoadShader(const char *vShaderFile, const char *fShaderFile);
Texture LoadTexture(const char *file);

int main(void)
{
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit())
    {
        const char *description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(800, 600, "2D Engine", nullptr, nullptr);
    if (!window)
    {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SPDLOG_ERROR("failed to initialize glad");
        glfwTerminate();
        return -1;
    }

    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", (const char*)glVersion);

    glViewport(0, 0, 800, 600);

    SpriteRenderer *render;
    Shader firstShader = LoadShader("./shader/vshader.vs", "./shader/fshader.fs");
    render = new SpriteRenderer(firstShader);

    // variables
    glm::vec2 position1 = glm::vec2(0.0f, 0.0f);
    glm::vec2 position2 = glm::vec2(0.5f, 0.0f);

    Texture texture1 = LoadTexture("./image/testImage.png");
    Texture texture2 = LoadTexture("./image/testImage2.png");

    firstShader.Use();

    // main loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render->DrawSprite(position1, texture1);
        render->DrawSprite(position2, texture2);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

std::optional<std::string> loadTextFile(const std::string &filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        SPDLOG_ERROR("failed to open file: {}", filename);
        return {};
    }
    std::stringstream text;
    text << fin.rdbuf();
    return text.str();
}

Shader LoadShader(const char *vShaderFile, const char *fShaderFile)
{
    auto vCode = loadTextFile(vShaderFile).value();
    auto fCode = loadTextFile(fShaderFile).value();
    const char *vCodePtr = vCode.c_str();
    const char *fCodePtr = fCode.c_str();
    int32_t vCodeLength = (int32_t)vCode.length();
    int32_t fCodeLength = (int32_t)fCode.length();

    Shader shader;
    shader.Compile(vCodePtr, fCodePtr);
    return shader;
}

Texture LoadTexture(const char *file)
{
    Texture texture;
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);

    texture.Generate(width, height, data);
    stbi_image_free(data);
    return texture;
}