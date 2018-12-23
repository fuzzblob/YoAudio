#pragma once

#include "YoaConfig.h"

#if SDPLOG_ENABLED
#include "spdlog/spdlog.h"
#endif // SDPLOG_ENABLED

class Log
{
public:
	static void Init();
#if SDPLOG_ENABLED
	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() noexcept { return m_logger; }
private:
	static std::shared_ptr<spdlog::logger> m_logger;
#endif // SDPLOG_ENABLED
};

// log macros
#if SDPLOG_ENABLED
#define YOA_ASSERT(x, ...)\
if (!(x))\
{\
YOA_CRITICAL("Assertion Failed: {0}"); \
__debugbreak(); \
} // no backslash
#define YOA_TRACE(...)			::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define YOA_INFO(...)			::Log::GetCoreLogger()->info(__VA_ARGS__)
#define YOA_WARN(...)			::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define YOA_ERROR(...)			::Log::GetCoreLogger()->error(__VA_ARGS__)
#define YOA_CRITICAL(...)	    ::Log::GetCoreLogger()->critical(__VA_ARGS__)
#else
// logging disabled (maybe the sdplog library was not found?)
// only errors and critical messages will be printed (and only the first argument :P)
#define YOA_ASSERT(x, ...)
#define YOA_TRACE(...)
#define YOA_INFO(...)
#define YOA_WARN(...)
#define YOA_ERROR(...)			printf(__VA_ARGS__)
#define YOA_CRITICAL(...)		printf(__VA_ARGS__)
#endif // SDPLOG_ENABLED
