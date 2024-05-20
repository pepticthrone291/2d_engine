#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glad/glad.h>

class Texture
{
public:
    uint32_t ID;
    uint32_t Width, Height;
    uint32_t Wrap_S = GL_CLAMP_TO_BORDER;
    uint32_t Wrap_T = GL_CLAMP_TO_BORDER;
    uint32_t Filter_Min = GL_NEAREST;
    uint32_t Filter_Max = GL_NEAREST;
    
    Texture();
    void Generate(uint32_t width, uint32_t height, unsigned char* data);
    void Bind() const;
};

#endif //   __TEXTURE_H__