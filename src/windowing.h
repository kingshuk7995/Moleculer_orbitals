#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLWindow {
private:
    GLFWwindow* m_window;
public:
    // Constructor and Destructor
    OpenGLWindow(int width, int height, const char* title);
    ~OpenGLWindow();

    inline void make_context_current() const {
        glfwMakeContextCurrent(m_window);
    }

    inline bool should_close() const {
        return glfwWindowShouldClose(m_window);
    }

    inline void swap_display_buffers() const {
        glfwSwapBuffers(m_window);
    }

    inline void poll_input_events() const {
        glfwPollEvents();
    }

    inline void terminate_glfw() const {
        glfwTerminate();
    }

    inline bool is_key_pressed(int key) const {
        return glfwGetKey(m_window, key) == GLFW_PRESS;
    }

    inline bool is_mouse_button_pressed(int button) const {
        return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
    }
    
    inline void on_framebuffer_resize() const {
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        });
    }

    inline void on_key_event(void (*callback)(GLFWwindow*, int, int, int, int)) const {
        glfwSetKeyCallback(m_window, callback);
    }

    inline void on_mouse_button_event(void (*callback)(GLFWwindow*, int, int, int)) const {
        glfwSetMouseButtonCallback(m_window, callback);
    }

    inline void on_cursor_move(void (*callback)(GLFWwindow*, double, double)) const {
        glfwSetCursorPosCallback(m_window, callback);
    }

    inline void on_scroll_event(void (*callback)(GLFWwindow*, double, double)) const {
        glfwSetScrollCallback(m_window, callback);
    }

    inline void on_cursor_enter(void (*callback)(GLFWwindow*, int)) const {
        glfwSetCursorEnterCallback(m_window, callback);
    }

    inline void on_file_drop(void (*callback)(GLFWwindow*, int, const char**)) const {
        glfwSetDropCallback(m_window, callback);
    }

    // Getter for raw window pointer
    inline GLFWwindow* raw_window() const {
        return m_window;
    }
};
