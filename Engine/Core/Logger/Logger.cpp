/**
 * @file Logger.cpp
 * @author Echo
 * @Date 2024/2/28
 * @brief
 */

#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

#define DEFAULT_LOGGER_PATTERN "[%Y-%m-%d %H:%M:%S] %^[%-8l]%$ [thread:%t] %v"

Logger g_logger = Logger("Global");

Logger::Logger() : Logger("Temp")
{
}

Logger::Logger(const std::string &logger_name) : Logger(logger_name, DEFAULT_LOGGER_PATTERN)
{
}

Logger::Logger(const std::string &logger_name, const std::string &pattern_string)
{
    m_logger = spdlog::stdout_color_mt(logger_name);
    m_pattern_string = pattern_string;
    m_logger->set_pattern(m_pattern_string);
}
