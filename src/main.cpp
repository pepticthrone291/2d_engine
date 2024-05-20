#include <iostream>

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <fstream>
#include <sstream>

void processInput(GLFWwindow *window);
std::optional<std::string> loadTextFile(const std::string &filename);

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

    // compile shaders and link to program
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

    auto vCode = loadTextFile("./shader/vshader.vs").value();
    const char *vCodePtr = vCode.c_str();
    int32_t vCodeLength = (int32_t)vCode.length();

    glShaderSource(vertexShader, 1, (const GLchar *const *)&vCodePtr, &vCodeLength);
    glCompileShader(vertexShader);

    int32_t success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to compile shader");
        SPDLOG_ERROR("reason: {}", infoLog);
        return false;
    }

    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    auto fCode = loadTextFile("./shader/fshader.fs").value();
    const char *fCodePtr = fCode.c_str();
    int32_t fCodeLength = (int32_t)fCode.length();

    glShaderSource(fragmentShader, 1, (const GLchar *const *)&fCodePtr, &fCodeLength);
    glCompileShader(fragmentShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to compile shader");
        SPDLOG_ERROR("reason: {}", infoLog);
        return false;
    }

    uint32_t shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to link program");
        SPDLOG_ERROR("reason: {}", infoLog);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // variables
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    uint32_t VBO;
    glGenBuffers(1, &VBO);
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);

    // main loop
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        glDrawArrays(GL_TRIANGLES, 0, 3);

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