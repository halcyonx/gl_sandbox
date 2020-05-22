#include "Log.h"

#ifndef PLATFORM_ANDROID
static std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
static std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Initialize()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_CoreLogger = spdlog::stdout_color_mt("CORE");
    s_CoreLogger->set_level(spdlog::level::trace);

    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);

    LOG_CORE_INFO("Log initialized");
}
#endif
