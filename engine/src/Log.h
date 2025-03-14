#pragma once

#include "YoaConfig.h"
#include <cassert>

#define YOA_ASSERT(x, ...) assert(x);
//#define YOA_ASSERT(x, ...) \
//{ \
//	YOA_CRITICAL("Assertion Failed! \n", __VA_ARGS__); \
//	assert(x); \
//}; \
//#if YOA_PLATFORM == YOA_WINDOWS \
//	/* MSVC specific debug break */ \
//	__debugbreak(); \
//#else \
//	/* POSIX portable debug break */ \
//	raise(SIGTRAP); \
//#endif \
//};

#if LOGGING_ENABLED
#if SPDLOG_FOUND
#include <spdlog/spdlog.h>
class Log
{
public:
	static void Init();
	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() noexcept { return mLogger; }
private:
	static std::shared_ptr<spdlog::logger> mLogger;
};

// log macros
#define YOA_TRACE(...)			::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define YOA_INFO(...)			::Log::GetCoreLogger()->info(__VA_ARGS__)
#define YOA_WARN(...)			::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define YOA_ERROR(...)			::Log::GetCoreLogger()->error(__VA_ARGS__)
#define YOA_CRITICAL(...)	    ::Log::GetCoreLogger()->critical(__VA_ARGS__)
#else
#include <stdio.h>
// sdplog library was not found
// only errors and critical messages will be printed (and only the first argument :P)
#define YOA_TRACE(...)
#define YOA_INFO(...)
#define YOA_WARN(...)
#define YOA_ERROR(...)			printf(#__VA_ARGS__)
#define YOA_CRITICAL(...)		printf(#__VA_ARGS__)
#endif
#else
// logging is disabled entirely
#define YOA_TRACE(...)
#define YOA_INFO(...)
#define YOA_WARN(...)
#define YOA_ERROR(...)
#define YOA_CRITICAL(...)
#endif
