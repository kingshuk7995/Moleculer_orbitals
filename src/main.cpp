#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#include <iostream>
#include <vector>
#include <string>

enum class Direction {
    Up, Down, Left, Right,
    UpLeft, UpRight, DownLeft, DownRight,
    None
};

const Direction find_dir(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        return Direction::UpLeft;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        return Direction::UpRight;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        return Direction::DownLeft;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        return Direction::DownRight;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        return Direction::Up;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        return Direction::Down;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        return Direction::Left;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        return Direction::Right;
    }
    else {
        return Direction::None;
    }
}

struct Atom {
    float x, y;
    float step_size;
    Atom(float init_x, float init_y) : x(init_x), y(init_y) {
        step_size = 0.001f;
    }
    void move(Direction dir) {
        switch (dir) {
        case Direction::Up:
            y += step_size;
            break;
        case Direction::Down:
            y -= step_size;
            break;
        case Direction::Left:
            x -= step_size;
            break;
        case Direction::Right:
            x += step_size;
            break;
        case Direction::UpLeft:
            x -= step_size * 0.707f;
            y += step_size * 0.707f;
            break;
        case Direction::UpRight:
            x += step_size * 0.707f;
            y += step_size * 0.707f;
            break;
        case Direction::DownLeft:
            x -= step_size * 0.707f;
            y -= step_size * 0.707f;
            break;
        case Direction::DownRight:
            x += step_size * 0.707f;
            y -= step_size * 0.707f;
            break;
        default:
            break;
        }
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    std::cout << "choose option : \n"
    << "visualizations options : \n"
    << "    1. 1s-1s interaction\n"
    << "    2. 2s-2s interaction\n"
    << "    3. 2p-2p interaction\n"
    << std::endl;
    int choice = -1;
    std::cin >> choice;
    const std::string frag_shader_path = [choice]() {
        switch (choice) {
        case 1:
            return "shaders/frag_1s_1s.glsl";
        case 2:
            return "shaders/frag_2s_2s.glsl";
        case 3:
            return "shaders/frag_2p_2p.glsl";
        default:
            std::cout << "invalid option\n"
                    << "choosing 1s-1s as default\n";
            return "shaders/frag_1s_1s.glsl";
        }
    }();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Molecular Orbital Visualization", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shaderProgram("shaders/vertex.glsl", frag_shader_path.c_str());

    float vertices[] = {
        -1.f,  1.f, 0.0f,  // Top-left
         1.f,  1.f, 0.0f,  // Top-right
        -1.f, -1.f, 0.0f,  // Bottom-left
         1.f, -1.f, 0.0f   // Bottom-right
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    Atom moving_atom = { 0.5f, 0.5f };

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();

        float windowWidth = 800.0f, windowHeight = 600.0f;
        float timeValue = static_cast<float>(glfwGetTime());

        glUniform2f(glGetUniformLocation(shaderProgram.get_id(), "uWindowSize"), windowWidth, windowHeight);
        glUniform1f(glGetUniformLocation(shaderProgram.get_id(), "uTime"), timeValue);

        const Direction dir = find_dir(window);
        moving_atom.move(dir);

        glUniform2f(glGetUniformLocation(shaderProgram.get_id(), "uMovingAtomPos"), moving_atom.x, moving_atom.y);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
