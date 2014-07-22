
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Log.h"

#include <fstream>
#include <ctime>
#include "OSSupport/IsThread.h"

#if defined(ANDROID_NDK)
	#include <android/log.h>
	#include "ToJava.h"
#endif




cLog* cLog::s_Log = NULL;

cLog::cLog(const AString & a_FileName)
	: m_File(NULL)
{
	s_Log = this;

	// create logs directory
	cFile::CreateFolder(FILE_IO_PREFIX + AString("logs"));

	OpenLog((FILE_IO_PREFIX + AString("logs/") + a_FileName).c_str());
}





cLog::~cLog()
{
	CloseLog();
	s_Log = NULL;
}





cLog * cLog::GetInstance()
{
	if (s_Log != NULL)
	{
		return s_Log;
	}

	new cLog("log.txt");
	return s_Log;
}





void cLog::CloseLog()
{
	if (m_File)
		fclose (m_File);
	m_File = 0;
}





void cLog::OpenLog( const char* a_FileName)
{
	if (m_File) fclose (m_File);
	#ifdef _MSC_VER
	fopen_s( &m_File, a_FileName, "a+");
	#else
	m_File = fopen(a_FileName, "a+");
	#endif
}





void cLog::ClearLog()
{
	#ifdef _MSC_VER
	if (fopen_s( &m_File, "log.txt", "w") == 0)
		fclose (m_File);
	#else
	m_File = fopen("log.txt", "w");
	if (m_File)
		fclose (m_File);
	#endif
	m_File = NULL;
}





void cLog::Log(const char * a_Format, va_list argList)
{
	AString Message;
	AppendVPrintf(Message, a_Format, argList);

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
	Printf(Line, "[%04lx|%02d:%02d:%02d] %s", cIsThread::GetCurrentID(), timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, Message.c_str());
	#else
	Printf(Line, "[%02d:%02d:%02d] %s", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, Message.c_str());
	#endif
	if (m_File)
	{
		fprintf(m_File, "%s\n", Line.c_str());
		fflush(m_File);
	}

	// Print to console:
#if defined(ANDROID_NDK)
	// __android_log_vprint(ANDROID_LOG_ERROR, "MCServer", a_Format, argList);
	__android_log_print(ANDROID_LOG_ERROR, "MCServer", "%s", Line.c_str());
	// CallJavaFunction_Void_String(g_JavaThread, "AddToLog", Line);
#else
	printf("%s", Line.c_str());
#endif

	#if defined (_WIN32) && defined(_DEBUG)
	// In a Windows Debug build, output the log to debug console as well:
	OutputDebugStringA((Line + "\n").c_str());
	#endif  // _WIN32
}





void cLog::Log(const char * a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	Log(a_Format, argList);
	va_end(argList);
}





void cLog::SimpleLog(const char * a_String)
{
	Log("%s", a_String);
}




