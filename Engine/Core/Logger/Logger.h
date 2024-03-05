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
    void Info(spdlog::format_string_t<Args...> fmt, Args &&...args) const  {
        m_logger->info(fmt, std::forward<Args>(args)...);
    }

    /** 输出Warning级别信息 */
    template <typename... Args>
    void Warning(spdlog::format_string_t<Args...> fmt, Args &&...args) const  {
        m_logger->warn(fmt, std::forward<Args>(args)...);
    }

    /** 输出Error级别信息 */
    template <typename... Args>
    void Error(spdlog::format_string_t<Args...> fmt, Args &&...args) const  {
        m_logger->error(fmt, std::forward<Args>(args)...);
    }

    /** 输出Debug级别信息 */
    template <typename... Args>
    void Debug(spdlog::format_string_t<Args...> fmt, Args &&...args) const  {
        m_logger->debug(fmt, std::forward<Args>(args)...);
    }

    /** 输出Trace级别信息 */
    template <typename... Args>
    void Trace(spdlog::format_string_t<Args...> fmt, Args &&...args) const  {
        m_logger->trace(fmt, std::forward<Args>(args)...);
    }

    /** 输出Critical级别信息 */
    template <typename... Args>
    void Critical(spdlog::format_string_t<Args...> fmt, Args &&...args) const {
        m_logger->critical(fmt, std::forward<Args>(args)...);
    }

    /**
     * 增加消息回调
     * @param sink
     */
    void AddCallback(const std::shared_ptr<spdlog::sinks::sink>& sink) const;

private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::string m_pattern_string;
};

extern const Logger g_logger;

#ifndef ZEPHYR_DEBUG
#define ZEPHYR_LOG_INFO(...) g_logger.Info(__VA_ARGS__);
#define ZEPHYR_LOG_WARNING(...) g_logger.Warning(__VA_ARGS__);
#define ZEPHYR_LOG_ERROR(...) g_logger.Error(__VA_ARGS__);
#define ZEPHYR_LOG_DEBUG(...) g_logger.Debug(__VA_ARGS__);
#define ZEPHYR_LOG_TRACE(...) g_logger.Trace(__VA_ARGS__);
#define ZEPHYR_LOG_CRITICAL(...) g_logger.Critical(__VA_ARGS__);
#else
#include <filesystem>
#include "Formatter.h"
#define ZEPHYR_LOG_INFO(Message, ...)      g_logger.Info("[{}:{}] [{}] " Message, std::filesystem::path(__FILE__).filename(), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define ZEPHYR_LOG_WARNING(Message, ...)   g_logger.Warning("[{}:{}] [{}] " Message, std::filesystem::path(__FILE__).filename(), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define ZEPHYR_LOG_ERROR(Message, ...)     g_logger.Error("[{}:{}] [{}] " Message, std::filesystem::path(__FILE__).filename(), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define ZEPHYR_LOG_TRACE(Message, ...)     g_logger.Trace("[{}:{}] [{}] " Message, std::filesystem::path(__FILE__).filename(), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define ZEPHYR_LOG_DEBUG(Message, ...)     g_logger.Debug("[{}:{}] [{}] " Message, std::filesystem::path(__FILE__).filename(), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define ZEPHYR_LOG_CRITICAL(Message, ...)  g_logger.Critical("[{}:{}] [{}] " Message, std::filesystem::path(__FILE__).filename(), __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif
#endif // ZEPHYR_LOGGER_H
