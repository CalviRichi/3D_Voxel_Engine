#include "vertexBuffer.h"
#include "commonInclude.h"
#include <iostream>

VertexBuffer::VertexBuffer() {
    
    std::cout << "VB generated" << std::endl;
    GLCALL(glGenBuffers(1, &m_RendererID));
}

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    GLCALL(glGenBuffers(1, &m_RendererID));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    std::cout << "deleting VBO" << std::endl;
    GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::Update(const void* data, unsigned int size, unsigned int offset) {
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCALL(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::setData(const void *data, unsigned int size) {
    Bind();
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}
