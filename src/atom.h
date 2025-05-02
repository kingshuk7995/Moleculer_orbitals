#pragma once
#include "windowing.h"

enum class Direction {
    Up, Down, Left, Right,
    UpLeft, UpRight, DownLeft, DownRight,
    None
};

const Direction find_dir(const OpenGLWindow& window);

struct Atom {
    float m_x, m_y;
    float m_h;
    Atom(float , float );
    
    void move(Direction );
};
