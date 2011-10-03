#include "cLog.h"
#include <fstream>
#include <ctime>
#include <stdarg.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/stat.h>   // for mkdir
#include <sys/types.h>

#define	sprintf_s(buffer, buffer_size, stringbuffer, ...) (sprintf(buffer, stringbuffer, __VA_ARGS__))
#define	vsnprintf_s(buffer, buffer_size, maxcount, stringbuffer, ...) (vsnprintf(buffer, maxcount, stringbuffer, __VA_ARGS__))
#endif

#include <string>

cLog* cLog::s_Log = NULL;

cLog::cLog( const char* a_FileName )
	: m_File(NULL)
{
	s_Log = this;

	// create logs directory
#ifdef _WIN32
	{
		SECURITY_ATTRIBUTES Attrib;
		Attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
		Attrib.lpSecurityDescriptor = NULL;
		Attrib.bInheritHandle = false;
		::CreateDirectory("logs", &Attrib);
	}
#else
	{
		mkdir("logs", S_IRWXU | S_IRWXG | S_IRWXO);
	}
#endif

	OpenLog( (std::string("logs/") + std::string(a_FileName)).c_str() );
}

cLog::~cLog()
{
	CloseLog();
	s_Log = NULL;
}

cLog* cLog::GetInstance()
{
	if(s_Log)
		return s_Log;

	new cLog("log.txt");
	return s_Log;
}

void cLog::CloseLog()
{
	if( m_File )
		fclose (m_File);
	m_File = 0;
}

void cLog::OpenLog( const char* a_FileName )
{
	if(m_File) fclose (m_File);
	#ifdef _WIN32
	fopen_s( &m_File, a_FileName, "a+" );
	#else
    m_File = fopen(a_FileName, "a+" );
	#endif
}

void cLog::ClearLog()
{
    #ifdef _WIN32
	if( fopen_s( &m_File, "log.txt", "w" ) == 0)
		fclose (m_File);
	#else
	m_File = fopen("log.txt", "w" );
	if( m_File )
		fclose (m_File);
	#endif
	m_File = 0;
}

void cLog::Log(const char* a_Format, va_list argList )
{
	char c_Buffer[1024];

	if( argList != 0 )
	{
		vsnprintf_s(c_Buffer, 1024, 1024, a_Format, argList );
	}
	else
	{
		sprintf_s( c_Buffer, 1024, "%s", a_Format );
	}

	time_t rawtime;
	time ( &rawtime );
#ifdef _WIN32
	struct tm timeinfo;
	localtime_s( &timeinfo, &rawtime );
#else
	struct tm* timeinfo;
	timeinfo = localtime( &rawtime );
#endif
	char c_Buffer2[1024];
#ifdef _WIN32
	sprintf_s(c_Buffer2, 1024, "[%02d:%02d:%02d] %s\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, c_Buffer);
#else
	sprintf(c_Buffer2, "[%02d:%02d:%02d] %s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, c_Buffer);
#endif
	if(m_File){
		fputs(c_Buffer2, m_File);
		fflush(m_File);
	}


	printf("%s", c_Buffer2 );
}

void cLog::Log(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	Log( a_Format, argList );
	va_end(argList);
}

void cLog::SimpleLog(const char* a_String)
{
	Log("%s", a_String );
}
