#pragma once

#include <spdlog/spdlog.h>

#include <memory>

/* To activate CORE logs */
#define SHOCK_USE_CORE_LOGGER

/* To activate CLIENT logs */
#define SHOCK_USE_CLIENT_LOGGER

/* To activate EVENT logs */
//#define SHOCK_LOG_EVENTS

namespace sbl
{
    class Log
    {
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_EventSystemLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    public:
        static void Init(const std::string& appName = "");
    public:
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetEventSystemLogger() { return s_EventSystemLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    };
}

#ifndef SBL_DIST
#define CORE_FATAL(...)   sbl::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CORE_ERROR(...)   sbl::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_WARN(...)    sbl::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_INFO(...)    sbl::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_TRACE(...)   sbl::Log::GetCoreLogger()->trace(__VA_ARGS__)
#else
#define CORE_FATAL(...)
#define CORE_ERROR(...)
#define CORE_WARN(...)
#define CORE_INFO(...)
#define CORE_TRACE(...)
#endif


#ifndef SBL_DIST
#define FATAL(...)        sbl::Log::GetClientLogger()->critical(__VA_ARGS__)
#define ERROR(...)        sbl::Log::GetClientLogger()->error(__VA_ARGS__)
#define WARN(...)         sbl::Log::GetClientLogger()->warn(__VA_ARGS__)
#define INFO(...)         sbl::Log::GetClientLogger()->info(__VA_ARGS__)
#define TRACE(...)        sbl::Log::GetClientLogger()->trace(__VA_ARGS__)
#else
#define FATAL(...)
#define ERROR(...)
#define WARN(...)
#define INFO(...)
#define TRACE(...)
#endif

#ifndef SBL_DIST
#define ASSERT(x, msg) if(!(x)){ CORE_FATAL(msg); __debugbreak();}
#else
#define ASSERT(x, msg) exit(0);
#endif