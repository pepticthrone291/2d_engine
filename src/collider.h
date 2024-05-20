#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

class Collider
{
public:
    Collider(Shader &shader);
    ~Collider();
    void Draw(glm::vec2 position, glm::vec2 size);
    bool colliderDrawn;
private:
    Shader shader;
    uint32_t quadVAO;
    void initRenderData();
};

#endif // __COLLIDER_H__