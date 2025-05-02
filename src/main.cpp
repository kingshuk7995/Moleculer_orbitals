#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"
#include "windowing.h"
#include "renderer.h"
#include <iostream>
#include "atom.h"

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
    OpenGLWindow window(800, 600, "OpenGL GLSL Triangle");
    window.on_framebuffer_resize();

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

    VertexArray va;
    va.bind();

    VertexBuffer vb(vertices, sizeof(vertices));
    IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int));

    va.add_layout(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
    va.unbind();

    Atom moving_atom = { 0.5f, 0.5f };

    Renderer renderer;

    while (!window.should_close()) {
        // Clear the screen
        renderer.clear(0.f, 0.f, 0.f, 1.0f);

        // Update uniforms
        float windowWidth = 800.0f, windowHeight = 600.0f;
        float timeValue = static_cast<float>(glfwGetTime());
        
        glUniform2f(glGetUniformLocation(shaderProgram.get_id(), "uWindowSize"), windowWidth, windowHeight);
        glUniform1f(glGetUniformLocation(shaderProgram.get_id(), "uTime"), timeValue);

        const Direction d = find_dir(window);
        moving_atom.move(d);

        glUniform2f(glGetUniformLocation(shaderProgram.get_id(), "uMovingAtomPos"), moving_atom.m_x, moving_atom.m_y);

        // Draw the scene
        renderer.draw(va, ib, shaderProgram);

        window.swap_display_buffers();
        window.poll_input_events();
    }

    return 0;
}
