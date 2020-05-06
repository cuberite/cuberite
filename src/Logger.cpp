
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "Logger.h"

#include "OSSupport/IsThread.h"
#ifdef _WIN32
	#include <time.h>
#endif





cLogger & cLogger::GetInstance(void)
{
	static cLogger Instance;
	return Instance;
}





void cLogger::InitiateMultithreading()
{
	GetInstance();
}





void cLogger::LogSimple(AString a_Message, eLogLevel a_LogLevel)
{
	time_t rawtime;
	time(&rawtime);

	struct tm * timeinfo;
	#ifdef _MSC_VER
		struct tm timeinforeal;
		timeinfo = &timeinforeal;
		localtime_s(timeinfo, &rawtime);
	#else
		timeinfo = localtime(&rawtime);
	#endif

	AString Line;
	#ifdef _DEBUG
		Printf(Line, "[%04llx|%02d:%02d:%02d] %s\n", static_cast<UInt64>(std::hash<std::thread::id>()(std::this_thread::get_id())), timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, a_Message);
	#else
		Printf(Line, "[%02d:%02d:%02d] %s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, a_Message);
	#endif


	cCSLock Lock(m_CriticalSection);
	for (size_t i = 0; i < m_LogListeners.size(); i++)
	{
		m_LogListeners[i]->Log(Line, a_LogLevel);
	}
}





void cLogger::vLogPrintf(const char * a_Format, eLogLevel a_LogLevel, fmt::printf_args a_ArgList)
{
	LogSimple(vPrintf(a_Format, a_ArgList), a_LogLevel);
}





void cLogger::vLogFormat(const char * a_Format, eLogLevel a_LogLevel, fmt::format_args a_ArgList)
{
	LogSimple(fmt::vformat(a_Format, a_ArgList), a_LogLevel);
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

void vFLOG(const char * a_Format, fmt::format_args a_ArgList)
{
	cLogger::GetInstance().vLogFormat(a_Format, cLogger::llRegular, a_ArgList);
}





void vFLOGINFO(const char * a_Format, fmt::format_args a_ArgList)
{
	cLogger::GetInstance().vLogFormat( a_Format, cLogger::llInfo, a_ArgList);
}





void vFLOGWARNING(const char * a_Format, fmt::format_args a_ArgList)
{
	cLogger::GetInstance().vLogFormat( a_Format, cLogger::llWarning, a_ArgList);
}





void vFLOGERROR(const char * a_Format, fmt::format_args a_ArgList)
{
	cLogger::GetInstance().vLogFormat(a_Format, cLogger::llError, a_ArgList);
}





void vLOG(const char * a_Format, fmt::printf_args a_ArgList)
{
	cLogger::GetInstance().vLogPrintf(a_Format, cLogger::llRegular, a_ArgList);
}





void vLOGINFO(const char * a_Format, fmt::printf_args a_ArgList)
{
	cLogger::GetInstance().vLogPrintf(a_Format, cLogger::llInfo, a_ArgList);
}





void vLOGWARNING(const char * a_Format, fmt::printf_args a_ArgList)
{
	cLogger::GetInstance().vLogPrintf(a_Format, cLogger::llWarning, a_ArgList);
}





void vLOGERROR(const char * a_Format, fmt::printf_args a_ArgList)
{
	cLogger::GetInstance().vLogPrintf( a_Format, cLogger::llError, a_ArgList);
}




