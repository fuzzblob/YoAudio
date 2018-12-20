#pragma once

#include "DllImportExport.h"

#include "spdlog/spdlog.h"

class YOA_API Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_logger; }
private:
	static std::shared_ptr<spdlog::logger> m_logger;
};

// log macros
#define YOA_TRACE(...)			::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define YOA_INFO(...)			::Log::GetCoreLogger()->info(__VA_ARGS__)
#define YOA_WARN(...)			::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define YOA_ERROR(...)			::Log::GetCoreLogger()->error(__VA_ARGS__)
#define YOA_CRITICAL(...)	    ::Log::GetCoreLogger()->critical(__VA_ARGS__)