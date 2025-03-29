#pragma once

#include "../include/EngineConfig.h"

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
#define YOA_TRACE(...)			{ auto log = ::Log::GetCoreLogger(); if(log)log->trace(__VA_ARGS__); };
#define YOA_INFO(...)			{ auto log = ::Log::GetCoreLogger(); if(log)log->info(__VA_ARGS__); };
#define YOA_WARN(...)			{ auto log = ::Log::GetCoreLogger(); if(log)log->warn(__VA_ARGS__); };
#define YOA_ERROR(...)			{ auto log = ::Log::GetCoreLogger(); if(log)log->error(__VA_ARGS__); };
#define YOA_CRITICAL(...)	    { auto log = ::Log::GetCoreLogger(); if(log)log->critical(__VA_ARGS__); };
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
