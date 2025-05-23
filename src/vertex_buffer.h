#pragma once
#include <glad/glad.h>

class VertexBuffer {
private:
    unsigned int m_id;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    void bind() const;
    void unbind() const;
};
