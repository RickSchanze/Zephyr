/**
 * @file ViewportWindow.cpp
 * @author Echo
 * @Date 24-3-3
 * @brief
 */

#include "ViewportWindow.h"

#include <imgui.h>
#ifndef __glad_h_
#include <glad/glad.h>
#endif

#include "Logger/Logger.h"
#include "Renderer/FrameRenderer.h"

ViewportWindow::ViewportWindow()
{
    m_frame_render = new FrameRenderer();
    m_frame_render->Initialize(1920, 1080);
    m_name = "Viewport";
}

void ViewportWindow::Draw()
{
    m_frame_render->Draw();
    const int width = static_cast<int>(ImGui::GetContentRegionAvail().x);
    const int height = static_cast<int>(ImGui::GetContentRegionAvail().y);
    Resize(width, height);

    ImGui::Image(reinterpret_cast<void *>(
                     m_frame_render->GetAttachTextureId()),
                 ImVec2(static_cast<float>(width), static_cast<float>(height)),
                 ImVec2(0, 1), ImVec2(1, 0));
}

void ViewportWindow::Resize(const int width, const int height)
{
    if (width == m_width && height == m_height)
        return;
    m_frame_render->Resize(width, height);
    glViewport(0, 0, width, height);
    m_width = width;
    m_height = height;
}
