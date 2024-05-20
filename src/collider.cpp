#include "collider.h"

Collider::Collider(glm::vec2 position, glm::vec2 size, Shader &shader)
    :Position(position), Size(size)
{
    this->shader = shader;
    this->initRenderData();
}

Collider::~Collider()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void Collider::Draw()
{
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(Position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::scale(model, glm::vec3(Size, 1.0f)); // last scale

    this->shader.SetMatrix4("model", model);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Collider::initRenderData()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f,
    };
    uint32_t indices[] = {
        0,  2,  1,  2,  0,  3,
    };
    uint32_t VBO, EBO;
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}