/**
 * @file Logger.cpp
 * @author Echo
 * @Date 2024/2/28
 * @brief
 */

#include "Logger.h"

#include "spdlog/sinks/callback_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#define DEFAULT_LOGGER_PATTERN "%^[%Y-%m-%d %H:%M:%S] [%-8l] [%-n] [thread:%t] %v%$"

const Logger g_logger = Logger("Main");

Logger::Logger() : Logger("Temp")
{

}

Logger::Logger(const std::string &logger_name) : Logger(logger_name, DEFAULT_LOGGER_PATTERN)
{
}

Logger::Logger(const std::string &logger_name, const std::string &pattern_string)
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    m_logger = std::make_shared<spdlog::logger>(logger_name, spdlog::sinks_init_list{console_sink});
    m_pattern_string = pattern_string;
    m_logger->set_pattern(m_pattern_string);
    m_logger->set_level(spdlog::level::trace);
}

void Logger::AddCallback(const std::shared_ptr<spdlog::sinks::sink>& sink) const
{
    m_logger->sinks().emplace_back(sink);
}
