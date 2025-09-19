#pragma once
//#include <GLFW/glfw3.h>
#include "vertexArray.h"
#include "vertexBufferLayout.h"
#include "indexBuffer.h"
#include "shader.h"
#include "vendor/glm/gtc/matrix_transform.hpp"

class Renderer {

public:
    
    Shader shader = Shader("res/shaders/basic.glsl");
    Shader highlight = Shader("res/shaders/highlight.glsl");
    Shader crosshair = Shader("res/shaders/crosshair.glsl");
    
    Renderer();
    ~Renderer();
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void drawBlockHighlight(glm::ivec3 blockPos, const glm::mat4& vp);
    void drawCrosshair();

};
