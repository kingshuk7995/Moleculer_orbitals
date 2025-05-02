#pragma once
#include <glad/glad.h>

class VertexArray {
private:
    unsigned int m_id;
public:
    VertexArray();
    ~VertexArray();
    void bind() const;

    void unbind() const;

    void add_layout(unsigned int index, int size, GLenum type, bool normalized,
         int stride, const void* offset) const;

};
