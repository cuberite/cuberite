
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Log.h"
#include "OSSupport/IsThread.h"



namespace Logger
{

	cLogDispacher & GetInstance(void)
	{
		static cLogDispacher Instance;
		return Instance;
	}

	void InitiateMultithreading()
	{
		GetInstance();
	}

	void cLogDispacher::LogSimple(AString a_Message, eLogLevel a_LogLevel)
	{
		time_t rawtime;
		time ( &rawtime);
	
		struct tm* timeinfo;
		#ifdef _MSC_VER
			struct tm timeinforeal;
			timeinfo = &timeinforeal;
			localtime_s(timeinfo, &rawtime);
		#else
			timeinfo = localtime( &rawtime);
		#endif

		AString Line;
		#ifdef _DEBUG
		Printf(Line, "[%04lx|%02d:%02d:%02d] %s\n", cIsThread::GetCurrentID(), timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, a_Message.c_str());
		#else
		Printf(Line, "[%02d:%02d:%02d] %s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, a_Message.c_str());
		#endif

		
		cCSLock Lock(m_CriticalSection);
		for(size_t i = 0; i < m_LogListeners.size(); i++)
		{
			m_LogListeners[i]->Log(a_Message, a_LogLevel);
		}
	}





	void cLogDispacher::Log(const char * a_Format, eLogLevel a_LogLevel, va_list a_ArgList)
	{
		AString Message;
		AppendVPrintf(Message, a_Format, a_ArgList);
		LogSimple(Message, a_LogLevel);
	}
	
	void cLogDispacher::AttachListener(Logger::cLoggerListener * a_Listener)
	{
		cCSLock Lock(m_CriticalSection);
		m_LogListeners.push_back(a_Listener);
	}
	
	void cLogDispacher::DetachListener(Logger::cLoggerListener * a_Listener)
	{
		cCSLock Lock(m_CriticalSection);
		m_LogListeners.erase(std::remove(m_LogListeners.begin(), m_LogListeners.end(), a_Listener));
	}
};



////////////////////////////////////////////////////////////////////////////////
// Global functions

void LOG(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	Logger::GetInstance().Log(a_Format, Logger::llRegular, argList);
	va_end(argList);
}

void LOGINFO(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	Logger::GetInstance().Log( a_Format, Logger::llInfo, argList);
	va_end(argList);
}

void LOGWARN(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	Logger::GetInstance().Log( a_Format, Logger::llWarning, argList);
	va_end(argList);
}

void LOGERROR(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	Logger::GetInstance().Log( a_Format, Logger::llError, argList);
	va_end(argList);
}




