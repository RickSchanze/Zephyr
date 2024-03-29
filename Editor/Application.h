/**
 * @file Application.h
 * @author Echo
 * @Date 2024/2/26
 * @brief 应用入口
 */

#ifndef ZEPHYR_APPLICATION_H
#define ZEPHYR_APPLICATION_H

#include "Event/Event.h"
#include "Logger/Logger.h"
#include "Widgets/ConsoleWindow.h"

class ViewportWindow;
struct GLFWwindow;

class Application
{
public:
    /**
     * 构造Application
     * @param width Application宽
     * @param height Application高
     */
    Application(const int width, const int height)
        : m_window(nullptr), m_width(width), m_height(height)
    {

    }

    /** 运行应用程序 */
    void Run();

protected:
#pragma region 初始化与结束函数

    /** 初始化ImGui */
    void InitializeImGui();
    /** 结束ImGui部分并进行清理工作 */
    void ShutdownImGui();

    /** 初始化GLFW */
    void InitializeGLFW();
    /** 结束GLFW部分并进行清理工作 */
    void ShutdownGLFW() const;

    /** 初始化其他部分 */
    void InitializeOther();
    /** 结束其他部分并进行清理工作 */
    void ShutdownOther();

#pragma endregion

    /** 程序主循环 */
    void MainLoop();
    /** 一帧开始渲染画面时调用 */
    void TickFrameRenderBegin();
    /** 渲染一帧的画面 */
    void TickFrameRender();
    /** 一帧结束渲染画面时调用 */
    void TickFrameRenderEnd();
    /** 一帧的逻辑运算开始时调用 */
    void TickFrameLogicBegin();
    /** 一帧的逻辑运算结束时调用 */
    void TickFrameLogicEnd();

private:
    GLFWwindow *m_window;       // 此Application对应GLFWWindow
    int m_width;                // 窗口宽
    int m_height;               // 窗口高

    // TODO: 删除并整理Window
    std::shared_ptr<ConsoleWindow> console_window;
    std::shared_ptr<ViewportWindow> viewport_window;
};


#endif // ZEPHYR_APPLICATION_H
