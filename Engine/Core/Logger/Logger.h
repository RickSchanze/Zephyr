/**
 * @file Logger.h
 * @author Echo
 * @Date 2024/2/28
 * @brief
 */

#ifndef ZEPHYR_LOGGER_H
#define ZEPHYR_LOGGER_H

#include <memory>
#include <string>

#include "spdlog/spdlog.h"

namespace spdlog
{
class logger;
}

class Logger
{
public:
    Logger();
    explicit Logger(const std::string& logger_name);
    Logger(const std::string& logger_name, const std::string& pattern_string);

    /** 输出Info级别信息 */
    template <typename... Args>
    void Info(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        m_logger->info(fmt, std::forward<Args>(args)...);
    }

    /** 输出Warning级别信息 */
    template <typename... Args>
    void Warning(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        m_logger->warn(fmt, std::forward<Args>(args)...);
    }

    /** 输出Error级别信息 */
    template <typename... Args>
    void Error(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        m_logger->error(fmt, std::forward<Args>(args)...);
    }

    /** 输出Debug级别信息 */
    template <typename... Args>
    void Debug(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        m_logger->debug(fmt, std::forward<Args>(args)...);
    }

    /** 输出Trace级别信息 */
    template <typename... Args>
    void Trace(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        m_logger->trace(fmt, std::forward<Args>(args)...);
    }

    /** 输出Critical级别信息 */
    template <typename... Args>
    void Critical(spdlog::format_string_t<Args...> fmt, Args &&...args) {
        m_logger->critical(fmt, std::forward<Args>(args)...);
    }

    /**
     * 增加消息回调
     * @param sink
     */
    void AddCallback(const std::shared_ptr<spdlog::sinks::sink>& sink);

private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::string m_pattern_string;
};

extern Logger g_logger;

#endif // ZEPHYR_LOGGER_H
