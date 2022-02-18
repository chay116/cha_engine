//
// Created by Chaeyoung Lim on 2022/02/16.
//

#ifndef CHA_ENGINE_LOGGER_H
#define CHA_ENGINE_LOGGER_H

#include <memory>
#include <spdlog/spdlog.h>

class Logger {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger;}
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger;}
private:
    static std::shared_ptr<spdlog::logger> m_CoreLogger;
    static std::shared_ptr<spdlog::logger> m_ClientLogger;
};

// Core log macros
#define CHA_CORE_TRACE(...) Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CHA_CORE_INFO(...) Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CHA_CORE_WARN(...) Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CHA_CORE_ERROR(...) Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CHA_CORE_FATAL(...) Logger::GetCoreLogger()->error(__VA_ARGS__)

// Core log macros
#define CHA_TRACE(...) Logger::GetClientLogger()->error(__VA_ARGS__)
#define CHA_INFO(...) Logger::GetClientLogger()->error(__VA_ARGS__)
#define CHA_WARN(...) Logger::GetClientLogger()->error(__VA_ARGS__)
#define CHA_ERROR(...) Logger::GetClientLogger()->error(__VA_ARGS__)
#define CHA_FATAL(...) Logger::GetClientLogger()->error(__VA_ARGS__)

#endif //CHA_ENGINE_LOGGER_H
