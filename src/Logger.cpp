//
// Created by Chaeyoung Lim on 2022/02/16.
//

#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> m_CoreLogger;
std::shared_ptr<spdlog::logger> m_ClientLogger;

void Logger::Init() {
    spdlog::set_pattern("%^[%T] %n : %v%$");
    m_CoreLogger = spdlog::stdout_color_mt("cha");
    m_CoreLogger->set_level(spdlog::level::trace);

    m_ClientLogger = spdlog::stdout_color_mt("APP");
    m_ClientLogger->set_level(spdlog::level::trace);
}