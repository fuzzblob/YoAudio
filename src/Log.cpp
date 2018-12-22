#include "Log.h"

#if SDPLOG_ENABLED
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::m_logger;

void Log::Init()
{
	m_logger = spdlog::stdout_color_mt("YOA");
	m_logger->set_level(spdlog::level::trace);

	spdlog::set_pattern("%^[%T] [%n:] %v%$");
	//spdlog::set_pattern("[%H:%M:%S] [%n] [%^-%L-%$] %v");

	YOA_WARN("YoAudio logging initialized\n[{0}:\t{1}]", __FILE__, __LINE__);
}
#endif // SDPLOG_ENABLED