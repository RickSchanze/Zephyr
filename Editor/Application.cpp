/**
 * @file Application.cpp
 * @author Echo 
 * @Date 2024/2/26
 * @brief 
 */

#include "Application.h"

#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Logger/Logger.h"

void Application::InitializeImGui() {
    // TODO: Log OpenGL版本
    //    LOG_INFO("OpenGL版本: {}", version);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto &io = ImGui::GetIO();
    (void) io;
    io.Fonts->AddFontFromFileTTF("c:/windows/fonts/consola.ttf", 24.0f, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    io.FontDefault = io.Fonts->Fonts[0];
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
                                                         //    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
                                                         //    Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void Application::ShutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::InitializeGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(m_width, m_height, "Zephyr Editor", nullptr, nullptr);
    if (m_window == nullptr) {
        // TODO: Log Here
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    // TODO: 鼠标回调
    //    glfwSetCursorPosCallback(m_window, &Application::MouseMoveCallback_GLFW);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwTerminate();
    }
}

void Application::ShutdownGLFW() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::InitializeOther() {
    console_window = std::make_shared<ConsoleWindow>();
}

void Application::ShutdownOther() {
}

void Application::Run() {
    g_logger.Info("Begin initialize application with width: {}, height: {}...", m_width, m_height);
    InitializeGLFW();
    InitializeImGui();
    InitializeOther();
    g_logger.Info("Initialize application complete.");
    g_logger.Trace("Application loop start.");
    g_logger.Debug("Application loop start.");
    g_logger.Info("Application loop start.");
    g_logger.Warning("Application loop start.");
    g_logger.Error("Application loop start.");
    g_logger.Critical("Application loop start.");
    MainLoop();
    g_logger.Info("Application loop end.");
    g_logger.Info("Deinitialize application...");
    ShutdownOther();
    ShutdownImGui();
    ShutdownGLFW();
    g_logger.Info("Deinitialize application complete.");
}

void Application::MainLoop() {
    while (!glfwWindowShouldClose(m_window)) {
        TickFrameLogicBegin();
        TickFrameLogicEnd();
        TickFrameRenderBegin();
        TickFrameRender();
        TickFrameRenderEnd();
    }
}

void Application::TickFrameRenderBegin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
}

void Application::TickFrameRenderEnd() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(m_window);
}

void Application::TickFrameLogicBegin() {
}

void Application::TickFrameLogicEnd() {
    glfwPollEvents();
}

void Application::TickFrameRender() {
    bool show = true;
    ImGui::ShowDemoWindow(&show);
    console_window->Render();
}
