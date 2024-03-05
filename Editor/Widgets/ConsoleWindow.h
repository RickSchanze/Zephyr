/**
 * @file ConsoleWindow.h
 * @author Echo
 * @Date 2024/2/29
 * @brief
 */

#ifndef ZEPHYR_CONSOLEWINDOW_H
#define ZEPHYR_CONSOLEWINDOW_H

#include <string>
#include <utility>
#include <vector>

#include "spdlog/details/log_msg.h"

#include "BaseWindow.h"
#include "Utils/TypeConverter.h"

struct LogMessage
{
    using TimePoint = std::chrono::system_clock::time_point;

    std::string message;
    int level;
    size_t thread_id;
    std::string logger_name;
    TimePoint time;

    LogMessage(std::string _message, const int _level, const size_t _thread_id, std::string _logger_name, const TimePoint &_time)
        : message(std::move(_message)), level(_level), thread_id(_thread_id), logger_name(std::move(_logger_name)), time(_time)
    {
    }

    explicit LogMessage(const spdlog::details::log_msg &msg)
        : LogMessage(TypeConverter::ToString(msg.payload), msg.level, msg.thread_id, TypeConverter::ToString(msg.logger_name), msg.time)
    {
    }
};

class ConsoleWindow : public BaseWindow
{
public:
    ConsoleWindow();

    void Draw() override;

protected:
    void OnReceiveLog(const spdlog::details::log_msg &msg);

private:
    std::vector<LogMessage> m_messages;
};

#endif // ZEPHYR_CONSOLEWINDOW_H
