#pragma once
#include <glad/glad.h>
#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

class Renderer {
public:
    void clear(float r, float g, float b, float a) const;
    void draw (const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
