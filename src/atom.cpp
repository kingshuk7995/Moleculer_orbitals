#include "atom.h"

const Direction find_dir(const OpenGLWindow& window) {
    if (window.is_key_pressed(GLFW_KEY_UP) && window.is_key_pressed(GLFW_KEY_LEFT)) {
        return Direction::UpLeft;
    }
    else if (window.is_key_pressed(GLFW_KEY_UP) && window.is_key_pressed(GLFW_KEY_RIGHT)) {
        return Direction::UpRight;
    }
    else if (window.is_key_pressed(GLFW_KEY_DOWN) && window.is_key_pressed(GLFW_KEY_LEFT)) {
        return Direction::DownLeft;
    }
    else if (window.is_key_pressed(GLFW_KEY_DOWN) && window.is_key_pressed(GLFW_KEY_RIGHT)) {
        return Direction::DownRight;
    }
    else if (window.is_key_pressed(GLFW_KEY_UP)) {
        return Direction::Up;
    }
    else if (window.is_key_pressed(GLFW_KEY_DOWN)) {
        return Direction::Down;
    }
    else if (window.is_key_pressed(GLFW_KEY_LEFT)) {
        return Direction::Left;
    }
    else if (window.is_key_pressed(GLFW_KEY_RIGHT)) {
        return Direction::Right;
    }
    else {
        return Direction::None;
    }
}

Atom::Atom(float x, float y) : m_x(x), m_y(y) {
    m_h = 0.001f;
}
void Atom::move(Direction d) {
    switch (d) {
    case Direction::Up:
        m_y += m_h;
        break;
    case Direction::Down:
        m_y -= m_h;
        break;
    case Direction::Left:
        m_x -= m_h;
        break;
    case Direction::Right:
        m_x += m_h;
        break;
    case Direction::UpLeft:
        m_x -= m_h * 0.707f;
        m_y += m_h * 0.707f;
        break;
    case Direction::UpRight:
        m_x += m_h * 0.707f;
        m_y += m_h * 0.707f;
        break;
    case Direction::DownLeft:
        m_x -= m_h * 0.707f;
        m_y -= m_h * 0.707f;
        break;
    case Direction::DownRight:
        m_x += m_h * 0.707f;
        m_y -= m_h * 0.707f;
        break;
    default:
        break;
    }
}
