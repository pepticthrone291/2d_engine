#ifndef __SPRITE_RENDERER_H__
#define __SPRITE_RENDERER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "texture.h"

class SpriteRenderer
{
public:
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();
    void DrawSprite(glm::vec2 position, Texture &texture);
private:
    Shader shader;
    uint32_t quadVAO;
    void initRenderData();
};

#endif  //__SPRITE_RENDERER_H__