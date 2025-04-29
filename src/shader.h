#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    unsigned int m_ID;
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    void use() const;
    inline unsigned int get_id() const {
        return m_ID;
    }
};
