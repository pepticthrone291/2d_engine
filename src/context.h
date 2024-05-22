#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(Context)
class Context
{
public:
    static ContextUPtr Create();
    void Render();
    void ProcessInput(GLFWwindow *window);
    void Reshape(int width, int height);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);
private:
    Context() {}
    bool Init();
    ProgramUPtr m_program;

    VertexLayoutUPtr m_vertexLayout;
    BufferUPtr m_vertexBuffer;
    BufferUPtr m_indexBuffer;

    // clear color
    glm::vec4 m_clearColor { glm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };

    int m_width {WINDOW_WIDTH};
    int m_height {WINDOW_HEIGHT};

    glm::vec3 position1 {glm::vec3(0.0f)};
    glm::vec3 size1 {glm::vec3(50.0f)};
    glm::vec3 position2 {glm::vec3(200.0f, 0.0f, 0.0f)};
};

#endif // __CONTEXT_H__