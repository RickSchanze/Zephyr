/**
 * @file BaseWindow.h
 * @author Echo
 * @Date 2024/2/29
 * @brief
 */

#ifndef ZEPHYR_BASEWINDOW_H
#define ZEPHYR_BASEWINDOW_H

#include <utility>

#include "WidgetBase.h"

class BaseWindow : public WidgetBase
{
public:
    /** BaseWindow构造函数,唯一区别就是默认draw_as_window为true */
    BaseWindow(std::string name, bool visible) : WidgetBase(std::move(name), visible, true) {}
    explicit BaseWindow(std::string name) : BaseWindow(std::move(name), true) {}
    BaseWindow() : BaseWindow(std::format("Window{}", s_widget_count)) {}

};

#endif // ZEPHYR_BASEWINDOW_H
