#include "collider.h"

Collider::Collider(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

Collider::~Collider()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void Collider::Draw(glm::vec2 position, glm::vec2 size)
{
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    this->shader.SetMatrix4("model", model);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);
}

void Collider::initRenderData()
{
    float vertices[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
    };

    uint32_t VBO;
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(1);
}