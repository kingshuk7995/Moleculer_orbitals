#include "vertex_array.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_id);
}
VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_id);
}
void VertexArray::bind() const {
    glBindVertexArray(m_id);
}
void VertexArray::unbind() const {
    glBindVertexArray(0);
}
void VertexArray::add_layout(unsigned int index, int size, GLenum type, bool normalized, int stride, const void* offset) const {
    glVertexAttribPointer(index, size, type, normalized ? GL_TRUE : GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
}
