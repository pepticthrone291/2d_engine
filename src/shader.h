#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <spdlog/spdlog.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    uint32_t ID;
    Shader() {}
    Shader &Use();
    void Compile(const char *vertexSource, const char *fragmentSource);
    void SetMatrix4(const std::string& name, const glm::mat4& value) const;
private:
    void checkCompileError(uint32_t shader, std::string type);
};

#endif  //__SHADER_H__