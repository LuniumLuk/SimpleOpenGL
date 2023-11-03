#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace SGL {

    static bool s_isImGuiInitialized = false;

    inline void initImGui(GLFWwindow* window) noexcept {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        const char* glsl_version = "#version 460";
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        s_isImGuiInitialized = true;
    }

    inline void shutdownImGui() noexcept {
        if (!s_isImGuiInitialized) return;
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    inline void preframeImGui() noexcept {
        if (!s_isImGuiInitialized) return;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    inline void endframeImGui() noexcept {
        if (!s_isImGuiInitialized) return;
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}
