#include "shader.h"

Shader &Shader::Use()
{
    glUseProgram(this->ID);
    return *this;
}

void Shader::Compile(const char *vertexSource, const char *fragmentSource)
{
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    checkCompileError(vertexShader, "VERTEX");

    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    checkCompileError(fragmentShader, "FRAGMENT");

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    glLinkProgram(this->ID);
    checkCompileError(this->ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::SetMatrix4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileError(uint32_t object, std::string type)
{
    int32_t success;
    char infoLog[1024];
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            SPDLOG_ERROR("failed to compile shader");
            SPDLOG_ERROR("reason: {}", infoLog);
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            SPDLOG_ERROR("failed to link program");
            SPDLOG_ERROR("reason: {}", infoLog);
        }
    }
}