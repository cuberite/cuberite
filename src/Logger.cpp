
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "Logger.h"

#include "OSSupport/IsThread.h"
#ifdef _WIN32
	#include <time.h>
#endif





static void WriteLogOpener(fmt::memory_buffer & Buffer)
{
	const time_t rawtime = time(nullptr);

	struct tm timeinfo;
#ifdef _MSC_VER
	localtime_s(&timeinfo, &rawtime);
#else
	localtime_r(&rawtime, &timeinfo);
#endif

#ifdef _DEBUG
	const auto ThreadID = std::hash<std::thread::id>()(std::this_thread::get_id());
	fmt::format_to(
		Buffer, "[{0:04x}|{1:02d}:{2:02d}:{3:02d}] ",
		ThreadID, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec
	);
#else
	fmt::format_to(
		Buffer, "[{0:02d}:{1:02d}:{2:02d}] ",
		timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec
	);
#endif
}





cLogger & cLogger::GetInstance(void)
{
	static cLogger Instance;
	return Instance;
}





void cLogger::InitiateMultithreading()
{
	GetInstance();
}





void cLogger::LogSimple(std::string_view a_Message, eLogLevel a_LogLevel)
{
	fmt::memory_buffer Buffer;
	WriteLogOpener(Buffer);
	fmt::format_to(Buffer, "{0}\n", a_Message);
	LogLine(std::string_view(Buffer.data(), Buffer.size()), a_LogLevel);
}





void cLogger::LogLine(std::string_view a_Line, eLogLevel a_LogLevel)
{
	cCSLock Lock(m_CriticalSection);
	for (size_t i = 0; i < m_LogListeners.size(); i++)
	{
		m_LogListeners[i]->Log(a_Line, a_LogLevel);
	}
}





void cLogger::LogPrintf(std::string_view a_Format, eLogLevel a_LogLevel, fmt::printf_args a_ArgList)
{
	fmt::memory_buffer Buffer;
	WriteLogOpener(Buffer);
	fmt::vprintf(Buffer, fmt::to_string_view(a_Format), a_ArgList);
	fmt::format_to(Buffer, "\n");

	LogLine(std::string_view(Buffer.data(), Buffer.size()), a_LogLevel);
}





void cLogger::LogFormat(std::string_view a_Format, eLogLevel a_LogLevel, fmt::format_args a_ArgList)
{
	fmt::memory_buffer Buffer;
	WriteLogOpener(Buffer);
	fmt::vformat_to(Buffer, a_Format, a_ArgList);
	fmt::format_to(Buffer, "\n");

	LogLine(std::string_view(Buffer.data(), Buffer.size()), a_LogLevel);
}





cLogger::cAttachment cLogger::AttachListener(std::unique_ptr<cListener> a_Listener)
{
	auto nonOwning = a_Listener.get();
	{
		cCSLock Lock(m_CriticalSection);
		m_LogListeners.push_back(std::move(a_Listener));
	}
	return cAttachment{nonOwning};
}





void cLogger::DetachListener(cListener * a_Listener)
{
	cCSLock Lock(m_CriticalSection);
	m_LogListeners.erase(
		std::remove_if(
			m_LogListeners.begin(),
			m_LogListeners.end(),
			[=](std::unique_ptr<cListener> & a_OtherListener) -> bool
			{
				return a_OtherListener.get() == a_Listener;
			}
		)
	);
}





////////////////////////////////////////////////////////////////////////////////
// Global functions

void Logger::LogFormat(std::string_view a_Format, eLogLevel a_LogLevel, fmt::format_args a_ArgList)
{
	cLogger::GetInstance().LogFormat(a_Format, a_LogLevel, a_ArgList);
}





void Logger::LogPrintf(std::string_view a_Format, eLogLevel a_LogLevel, fmt::printf_args a_ArgList)
{
	cLogger::GetInstance().LogPrintf(a_Format, a_LogLevel, a_ArgList);
}





void Logger::LogSimple(std::string_view a_Message, eLogLevel a_LogLevel)
{
	cLogger::GetInstance().LogSimple(a_Message, a_LogLevel);
}
