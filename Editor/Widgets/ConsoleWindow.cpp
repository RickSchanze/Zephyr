/**
 * @file ConsoleWindow.cpp
 * @author Echo
 * @Date 2024/2/29
 * @brief
 */

#include "ConsoleWindow.h"

#include "spdlog/sinks/callback_sink.h"

#include "Logger/Logger.h"
#include "imgui.h"

// {66, 186, 144, 1}}
#define LOG_COLOR_INFO                                             \
    {                                                              \
        0.2588235294117f, 0.7294117647058f, 0.5647058823529f, 1.0f \
    }
// {164, 132, 75, 1}
#define LOG_COLOR_WARNING                                         \
    {                                                             \
        0.643137254902f, 0.5176470588235f, 0.2941176470588f, 1.0f \
    }
// {179, 63, 57, 1}
#define LOG_COLOR_ERROR                                            \
    {                                                              \
        0.7019607843137f, 0.2470588235294f, 0.2235294117647f, 1.0f \
    }
// {77, 199, 228, 1}
#define LOG_COLOR_DEBUG                                            \
    {                                                              \
        0.3019607843137f, 0.7803921568627f, 0.8941176470588f, 1.0f \
    }
// {183, 185, 162, 1}
#define LOG_COLOR_TRACE                                            \
    {                                                              \
        0.7176470588235f, 0.7254901960784f, 0.6352941176471f, 1.0f \
    }
// 红色
#define LOG_COLOR_CRITICAL \
    {                      \
        1, 0, 0, 1         \
    }

ImVec4 GetLogColor(const int log_level)
{
    switch (log_level)
    {
    case 0:
        return LOG_COLOR_TRACE;
    case 1:
        return LOG_COLOR_DEBUG;
    case 2:
        return LOG_COLOR_INFO;
    case 3:
        return LOG_COLOR_WARNING;
    case 4:
        return LOG_COLOR_ERROR;
    case 5:
        return LOG_COLOR_CRITICAL;
    default:
        return {1, 1, 1, 1};
    }
}

const char *GetLogLevelString(const int log_level)
{
    switch (log_level)
    {
    case 0:
        return "Trace";
    case 1:
        return "Debug";
    case 2:
        return "Info";
    case 3:
        return "Warning";
    case 4:
        return "Error";
    case 5:
        return "Critical";
    default:
        return "Unknown";
    }
}

void ConsoleWindow::Draw()
{
    for (const auto &msg : m_messages)
    {
        std::string output_string = std::format("[{:%Y-%m-%d %H:%M}] [{:<8}] [{}] [Thread:{}] {}", msg.time ,GetLogLevelString(msg.level), msg.logger_name, msg.thread_id, msg.message);
        ImGui::TextColored(GetLogColor(msg.level), "%s", output_string.c_str());
    }
}

void ConsoleWindow::OnReceiveLog(const spdlog::details::log_msg &msg)
{
    m_messages.emplace_back(msg);
}

ConsoleWindow::ConsoleWindow()
{
    m_name = "Console";
    g_logger.AddCallback(std::make_shared<spdlog::sinks::callback_sink_mt>([this](const spdlog::details::log_msg &msg) {
        OnReceiveLog(msg);
    }));
}
