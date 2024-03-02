/**
 * @file WidgetBase.cpp
 * @author Echo
 * @Date 2024/2/29
 * @brief
 */

#include "WidgetBase.h"

#include "imgui.h"

void WidgetBase::Render()
{
    if (m_is_draw_as_window)
    {
        ImGui::Begin(m_name.c_str(), &m_visible, ImGuiWindowFlags_HorizontalScrollbar);
    }

    Draw();

    if (m_is_draw_as_window)
    {
        ImGui::End();
    }
}
bool WidgetBase::IsVisible() const
{
    return m_visible;
}
void WidgetBase::SetVisible(const bool visible)
{
    m_visible = visible;
}

void WidgetBase::SetDrawAsWindow(const bool is_draw_as_window)
{
    m_is_draw_as_window = is_draw_as_window;
}

bool WidgetBase::IsDrawAsWindow() const
{
    return m_is_draw_as_window;
}

std::string WidgetBase::GetName() const
{
    return m_name;
}
