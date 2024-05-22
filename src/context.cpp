#include "context.h"

ContextUPtr Context::Create()
{
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

void Context::ProcessInput(GLFWwindow* window)
{
    move = glm::vec2(0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        move.y += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        move.y -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        move.x += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        move.x -= 1.0f;

    if (move != glm::vec2(0.0f, 0.0f))
    {
        move = moveSpeed * glm::normalize(move);
    }

    newPosition1.x = position1.x + move.x;
    newPosition1.y = position1.y + move.y;

    // 충돌 체크 후 실제 위치 갱신
    if (move.x != 0)
    {
        newPosition1.x = position1.x + move.x;
        if (!Collision(glm::vec3(newPosition1.x, position1.y, position1.z), size1, position2, size1))
            position1.x = newPosition1.x;
        else move.x = 0.0f;
    }
    if (move.y != 0)
    {
        newPosition1.y = position1.y + move.y;
        if (!Collision(glm::vec3(position1.x, newPosition1.y, position1.z), size1, position2, size1))
            position1.y = newPosition1.y;
        else move.y = 0.0f;
    }

    SPDLOG_INFO("{}, {}", move.x, move.y);
}

void Context::Reshape(int width, int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}

void Context::MouseMove(double x, double y)
{
}

void Context::MouseButton(int button, int action, double x, double y)
{
    
}

bool Context::Collision(glm::vec3 position1, glm::vec3 size1, glm::vec3 position2, glm::vec3 size2)
{
    float xThick = (size1.x + size2.x) / 2.0f;
    float yThick = (size1.y + size2.y) / 2.0f;

    float xDis = position1.x - position2.x;
    float yDis = position1.y - position2.y;

    if (xDis <= xThick && xDis >= -xThick && yDis <= yThick && yDis >= -yThick)
        return true;
    else return false;
}

bool Context::Init()
{
    glClearColor(0.2f, 0.2f, 0.0f, 1.0f);

    ShaderPtr vertexShader = Shader::CreateFromFile("./shader/vshader.vs", GL_VERTEX_SHADER);
    ShaderPtr fragmentShader = Shader::CreateFromFile("./shader/fshader.fs", GL_FRAGMENT_SHADER);
    SPDLOG_INFO("vertex shader id: {}", vertexShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragmentShader->Get());
    m_program = Program::Create({vertexShader, fragmentShader});

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
    };

    uint32_t indices[] = {
        0, 1, 2, 3
    };

    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));

    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    
    return true;
}

void Context::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
        
    m_program->Use();   
    auto projection = glm::ortho(-0.5f * WINDOW_WIDTH, 0.5f * WINDOW_WIDTH, -0.5f * WINDOW_HEIGHT, 0.5f * WINDOW_HEIGHT, -1.0f, 1.0f);
    auto model = glm::translate(glm::mat4(1.0f), position1);
    model = glm::scale(model, size1);
    auto transform = projection * model;
    m_program->SetUniform("transform", transform);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    auto model2 = glm::translate(glm::mat4(1.0f), position2);
    model2 = glm::scale(model2, size1);
    auto transform2 = projection * model2;
    m_program->SetUniform("transform", transform2);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}