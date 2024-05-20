#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "shader.h"
#include "sprite_renderer.h"
#include "texture.h"
#include "collider.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
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
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "2D Engine", nullptr, nullptr);
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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    auto imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    SpriteRenderer *render;
    Shader firstShader = LoadShader("./shader/vshader.vs", "./shader/fshader.fs");
    render = new SpriteRenderer(firstShader);

    auto projection = glm::ortho(-0.5f * (float)WINDOW_WIDTH, 0.5f * (float)WINDOW_WIDTH, -0.5f * (float)WINDOW_HEIGHT, 0.5f * (float)WINDOW_HEIGHT, -1.0f, 1.0f);
    firstShader.Use().SetMatrix4("projection", projection);
    

    // variables
    glm::vec2 position1 {glm::vec2(0.0f, 0.0f)};
    glm::vec2 position2 {glm::vec2(200.0f, 0.0f)};
    glm::vec2 size1 {glm::vec2(100.0f, 100.0f)};
    glm::vec2 size2 {glm::vec2(100.0f, 100.0f)};

    Collider *collider1;
    Shader shader2 = LoadShader("./shader/collvshader.vs", "./shader/collfshader.fs");
    collider1 = new Collider(shader2);
    shader2.Use().SetMatrix4("projection", projection);

    float moveSpeed {2.0f};

    Texture texture1 = LoadTexture("./image/testImage.png");
    Texture texture2 = LoadTexture("./image/testImage2.png");

    firstShader.Use();

    // main loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position1.x += moveSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position1.x -= moveSpeed;

        if (ImGui::Begin("physical simulation test"))
        {
            ImGui::DragFloat("move speed", &moveSpeed, 0.02f, 0.0f, 50.0f);
        }
        ImGui::End();

        glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        render->DrawSprite(position1, size1, texture1);
        render->DrawSprite(position2, size2, texture2);
        collider1->Draw(position1, size1);

        glfwPollEvents();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiContext);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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