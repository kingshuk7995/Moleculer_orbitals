#include "vertex_array.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"
#include "windowing.h"
#include "renderer.h"
#include <iostream>
#include <memory>
#include <cmath>
#include "atom.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int main() {
    OpenGLWindow window(1024, 768, "Molecular Orbitals Visualizer");
    window.on_framebuffer_resize();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.raw_window(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // Load generalized shader
    std::unique_ptr<Shader> shader_general = std::make_unique<Shader>("shaders/vertex.glsl", "shaders/frag_general.glsl");

    Shader* current_shader = shader_general.get();

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

    // Simulation params
    float time_multiplier = 1.0f;
    float intensity_scale = 10.0f;
    float simulation_time = 0.0f;
    float phase_diff = 0.0f;
    float bohr_radius = 0.15f;

    // Quantum numbers
    int n1 = 1, l1 = 0, m1 = 0;
    int n2 = 1, l2 = 0, m2 = 0;

    // Rotation
    float pitch = 0.0f; // x-axis rotation
    float yaw = 0.0f;   // y-axis rotation
    float roll = 0.0f;  // z-axis rotation

    while (!window.should_close()) {
        window.poll_input_events();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui UI
        static bool first_frame = true;
        if (first_frame) {
            ImGui::SetNextWindowPos(ImVec2(1050, 50), ImGuiCond_Once);
            first_frame = false;
        }
        ImGui::Begin("Molecular Orbitals Settings");
        ImGui::Text("Explore any hydrogenic interaction.");
        ImGui::Separator();
        
        ImGui::Text("Atom 1 (Fixed):");
        ImGui::SliderInt("n1", &n1, 1, 5);
        if (l1 >= n1) l1 = n1 - 1;
        ImGui::SliderInt("l1", &l1, 0, n1 - 1);
        if (m1 > l1) m1 = l1;
        if (m1 < -l1) m1 = -l1;
        ImGui::SliderInt("m1", &m1, -l1, l1);

        ImGui::Separator();
        ImGui::Text("Atom 2 (Moving):");
        ImGui::SliderInt("n2", &n2, 1, 5);
        if (l2 >= n2) l2 = n2 - 1;
        ImGui::SliderInt("l2", &l2, 0, n2 - 1);
        if (m2 > l2) m2 = l2;
        if (m2 < -l2) m2 = -l2;
        ImGui::SliderInt("m2", &m2, -l2, l2);

        ImGui::Separator();
        ImGui::SliderFloat("Simulation Speed", &time_multiplier, 0.0f, 5.0f);
        ImGui::SliderFloat("Intensity Scale", &intensity_scale, 0.1f, 50.0f);
        ImGui::SliderFloat("Phase Difference", &phase_diff, 0.0f, 6.2831853f);
        ImGui::SliderFloat("Bohr Radius", &bohr_radius, 0.05f, 0.5f);

        ImGui::Separator();
        ImGui::Text("3D Rotation (Slice Plane)");
        ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
        ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
        ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);

        ImGui::Separator();
        ImGui::Text("Moving Atom Position");
        ImGui::SliderFloat("X", &moving_atom.m_x, 0.0f, 1.0f);
        ImGui::SliderFloat("Y", &moving_atom.m_y, 0.0f, 1.0f);
        
        ImGui::End();

        // Update simulation
        simulation_time += static_cast<float>(time_multiplier * 0.016f); // Assuming ~60fps
        const Direction d = find_dir(window);
        moving_atom.move(d);

        // Clear the screen
        renderer.clear(0.f, 0.f, 0.f, 1.0f);

        // Update uniforms
        int display_w, display_h;
        glfwGetFramebufferSize(window.raw_window(), &display_w, &display_h);
        float windowWidth = static_cast<float>(display_w);
        float windowHeight = static_cast<float>(display_h);

        current_shader->use();

        glUniform2f(glGetUniformLocation(current_shader->get_id(), "uWindowSize"), windowWidth, windowHeight);
        glUniform1f(glGetUniformLocation(current_shader->get_id(), "uTime"), simulation_time);
        glUniform2f(glGetUniformLocation(current_shader->get_id(), "uMovingAtomPos"), moving_atom.m_x, moving_atom.m_y);
        glUniform1f(glGetUniformLocation(current_shader->get_id(), "uIntensityScale"), intensity_scale);
        glUniform1f(glGetUniformLocation(current_shader->get_id(), "uPhaseDiff"), phase_diff);
        glUniform1f(glGetUniformLocation(current_shader->get_id(), "uBohrRadius"), bohr_radius);

        glUniform1i(glGetUniformLocation(current_shader->get_id(), "uN1"), n1);
        glUniform1i(glGetUniformLocation(current_shader->get_id(), "uL1"), l1);
        glUniform1i(glGetUniformLocation(current_shader->get_id(), "uM1"), m1);

        glUniform1i(glGetUniformLocation(current_shader->get_id(), "uN2"), n2);
        glUniform1i(glGetUniformLocation(current_shader->get_id(), "uL2"), l2);
        glUniform1i(glGetUniformLocation(current_shader->get_id(), "uM2"), m2);

        // Compute rotation matrix
        float cp = cos(pitch), sp = sin(pitch);
        float cy = cos(yaw), sy = sin(yaw);
        float cr = cos(roll), sr = sin(roll);

        // Z-Y-X rotation matrix
        float rotMat[9] = {
            cy * cr, cy * sr, -sy,
            sp * sy * cr - cp * sr, sp * sy * sr + cp * cr, sp * cy,
            cp * sy * cr + sp * sr, cp * sy * sr - sp * cr, cp * cy
        };
        glUniformMatrix3fv(glGetUniformLocation(current_shader->get_id(), "uRotation"), 1, GL_FALSE, rotMat);

        // Draw the scene
        renderer.draw(va, ib, *current_shader);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        window.swap_display_buffers();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
