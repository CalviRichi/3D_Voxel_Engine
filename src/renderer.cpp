#include "renderer.h"

Renderer::Renderer() {}

Renderer::~Renderer() {
    shader.Unbind(); // "shader" is the general block shader
    highlight.Unbind(); // this is for highlighting the block the player is looking at
    crosshair.Unbind(); // this is the crosshair 
}

void Renderer::clear() const {
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    //shader.Bind(); //THIS IS ALREADY DONE BEFORE THE DRAW CALL
    va.Bind(); // only this one is completely necessary
    ib.Bind();   

    GLCALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    // this is assuming that unsigned int is hard coded into the renderer
    //GLCALL(glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawBlockHighlight(glm::ivec3 blockPos, const glm::mat4& vp) {
    static GLuint vao = 0, vbo = 0;
    if (!vao) {
        // A simple cube made of 36 vertices (12 triangles)
        float cubeVerts[] = {
            // pos only, you don’t need texcoords/normals for a flat color
            // Front
            0,0,1,  1,0,1,  1,1,1,
            0,0,1,  1,1,1,  0,1,1,
            // Back
            0,0,0,  0,1,0,  1,1,0,
            0,0,0,  1,1,0,  1,0,0,
            // Left
            0,0,0,  0,0,1,  0,1,1,
            0,0,0,  0,1,1,  0,1,0,
            // Right
            1,0,0,  1,1,0,  1,1,1,
            1,0,0,  1,1,1,  1,0,1,
            // Top
            0,1,0,  0,1,1,  1,1,1,
            0,1,0,  1,1,1,  1,1,0,
            // Bottom
            0,0,0,  1,0,0,  1,0,1,
            0,0,0,  1,0,1,  0,0,1
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(blockPos));
    model = glm::scale(model, glm::vec3(1.01));
    glm::mat4 mvp   = vp * model;

    highlight.Bind();
    highlight.setUniformMat4f("u_MVP", mvp);
    highlight.setUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 0.3f); // yellow tint, 30% opaque
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glDisable(GL_CULL_FACE); // so all sides are visible
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    
    glDisable(GL_BLEND);
    //glEnable(GL_CULL_FACE);
}

void Renderer::drawCrosshair() {
    static GLuint cross_vao = 0, cross_vbo = 0;
    if (!cross_vao) {
        // A simple cube made of 36 vertices (12 triangles)
        float crossVerts[] = {
            -0.02f, 0.0f,  // left
             0.02f, 0.0f,  // right
             0.0f, -0.02f, // bottom
             0.0f, 0.02f   // top
        };

        glGenVertexArrays(1, &cross_vao);
        glGenBuffers(1, &cross_vbo);
        glBindVertexArray(cross_vao);
        glBindBuffer(GL_ARRAY_BUFFER, cross_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(crossVerts), crossVerts, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }
    
    crosshair.Bind();
    crosshair.setUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f); // RGBA

    glDisable(GL_DEPTH_TEST);
    glLineWidth(2.0f);

    glBindVertexArray(cross_vao);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    glLineWidth(1.0f);
    glEnable(GL_DEPTH_TEST);
}
