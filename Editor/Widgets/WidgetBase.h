/**
 * @file WidgetBase.h
 * @author Echo
 * @Date 2024/2/29
 * @brief
 */

#ifndef ZEPHYR_WIDGETBASE_H
#define ZEPHYR_WIDGETBASE_H

#include <format>
#include <string>

class WidgetBase
{
public:
    /**
     * 构造Widget
     * @note 当is_draw_as_window为false时,此Widget必须被包含在一个Window(IsDrawAsWindow()==true)里
     * @param visible widget是否可见
     * @param is_draw_as_window 是否以窗口形式绘制(是否加入ImGui::Begin)
     * @param name widget名称
     */
    WidgetBase(std::string name, const bool visible, const bool is_draw_as_window)
        : m_visible(visible), m_is_draw_as_window(is_draw_as_window), m_name(std::move(name))
    {
        s_widget_count++;
    }

    /**
     * 构造Widget
     * @note 当is_draw_as_window为false时,此Widget必须被包含在一个Window(IsDrawAsWindow()==true)里
     * @param visible widget是否可见
     * @param name widget名称
     */
    WidgetBase(std::string name, const bool visible)
        : WidgetBase(std::move(name), visible, false)
    {
    }

    /**
     * 构造Widget
     * @note 当is_draw_as_window为false时,此Widget必须被包含在一个Window(IsDrawAsWindow()==true)里
     * @param name widget名称
     */
    explicit WidgetBase(std::string name)
        : WidgetBase(std::move(name), true, false)
    {
    }

    /** 默认构造 */
    WidgetBase()
        : WidgetBase(std::format("Widget{}", s_widget_count))
    {
    }

    virtual ~WidgetBase() = default;

    /** 渲染Widget,当m_is_draw_as_window==true时自动加ImGui::Begin()与ImGui::End() */
    void Render();
    /** 子类应覆写此方法来实现UI */
    virtual void Draw() = 0;
    /** 设置可见性 */
    inline void SetVisible(bool visible);
    /** 是否可见 */
    inline bool IsVisible() const;
    /** 是否以窗口形式绘制 */
    inline bool IsDrawAsWindow() const;
    /** 设置是否以窗口形式绘制 */
    inline void SetDrawAsWindow(bool is_draw_as_window);
    /** 获取名称 */
    inline std::string GetName() const;

protected:
    bool m_visible = true;
    bool m_is_draw_as_window = false;
    std::string m_name;
    static inline int s_widget_count = 0;
};

#endif // ZEPHYR_WIDGETBASE_H
