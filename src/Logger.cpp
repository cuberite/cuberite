
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
		Printf(Line, "[%04llx|%02d:%02d:%02d] %s\n", static_cast<UInt64>(std::hash<std::thread::id>()(std::this_thread::get_id())), timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, a_Message.c_str());
	#else
		Printf(Line, "[%02d:%02d:%02d] %s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, a_Message.c_str());
	#endif


	cCSLock Lock(m_CriticalSection);
	for (size_t i = 0; i < m_LogListeners.size(); i++)
	{
		m_LogListeners[i]->Log(Line, a_LogLevel);
	}
}





void cLogger::LogPrintf(const char * a_Format, eLogLevel a_LogLevel, fmt::ArgList a_ArgList)
{
	LogSimple(Printf(a_Format, a_ArgList), a_LogLevel);
}





void cLogger::LogFormat(const char * a_Format, eLogLevel a_LogLevel, fmt::ArgList a_ArgList)
{
	LogSimple(fmt::format(a_Format, a_ArgList), a_LogLevel);
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

void FLOG(const char * a_Format, fmt::ArgList a_ArgList)
{
	cLogger::GetInstance().LogFormat(a_Format, cLogger::llRegular, a_ArgList);
}





void FLOGINFO(const char * a_Format, fmt::ArgList a_ArgList)
{
	cLogger::GetInstance().LogFormat( a_Format, cLogger::llInfo, a_ArgList);
}





void FLOGWARNING(const char * a_Format, fmt::ArgList a_ArgList)
{
	cLogger::GetInstance().LogFormat( a_Format, cLogger::llWarning, a_ArgList);
}





void FLOGERROR(const char * a_Format, fmt::ArgList a_ArgList)
{
	cLogger::GetInstance().LogFormat( a_Format, cLogger::llError, a_ArgList);
}





void LOG(const char * a_Format, fmt::ArgList a_ArgList)
{
	cLogger::GetInstance().LogPrintf(a_Format, cLogger::llRegular, a_ArgList);
}





void LOGINFO(const char * a_Format, fmt::ArgList a_ArgList)
{
	cLogger::GetInstance().LogPrintf( a_Format, cLogger::llInfo, a_ArgList);
}





void LOGWARNING(const char * a_Format, fmt::ArgList a_ArgList)
{
	cLogger::GetInstance().LogPrintf( a_Format, cLogger::llWarning, a_ArgList);
}





void LOGERROR(const char * a_Format, fmt::ArgList a_ArgList)
{
	cLogger::GetInstance().LogPrintf( a_Format, cLogger::llError, a_ArgList);
}




