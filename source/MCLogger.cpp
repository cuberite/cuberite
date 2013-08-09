
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include <time.h>
#include "Log.h"





cMCLogger * cMCLogger::s_MCLogger = NULL;

#ifdef _WIN32
	HANDLE g_Console = GetStdHandle(STD_OUTPUT_HANDLE);
#endif




cMCLogger* cMCLogger::GetInstance()
{
	return s_MCLogger;
}





cMCLogger::cMCLogger(void)
{
	AString FileName;
	Printf(FileName, "LOG_%d.txt", (int)time(NULL));
	m_Log = new cLog(FileName);
	m_Log->Log("--- Started Log ---");

	s_MCLogger = this;
}





cMCLogger::cMCLogger(const AString & a_FileName)
{
	m_Log = new cLog(a_FileName);
}





cMCLogger::~cMCLogger()
{
	m_Log->Log("--- Stopped Log ---");
	delete m_Log;
	if (this == s_MCLogger)
		s_MCLogger = NULL;
}





void cMCLogger::LogSimple(const char* a_Text, int a_LogType /* = 0 */ )
{
	switch( a_LogType )
	{
		case 0:
			LOG("%s", a_Text);
			break;
		case 1:
			LOGINFO("%s", a_Text);
			break;
		case 2:
			LOGWARN("%s", a_Text);
			break;
		case 3:
			LOGERROR("%s", a_Text);
			break;
		default:
			LOG("(#%d#: %s", a_LogType, a_Text);
			break;
	}
}





void cMCLogger::Log(const char * a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
	SetColor(csGrayOnBlack);
	m_Log->Log(a_Format, a_ArgList);
}





void cMCLogger::Info(const char * a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
	SetColor(csYellowOnBlack);
	m_Log->Log(a_Format, a_ArgList);
}





void cMCLogger::Warn(const char * a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
	SetColor(csRedOnBlack);
	m_Log->Log(a_Format, a_ArgList);
}





void cMCLogger::Error(const char * a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
	SetColor(csBlackOnRed);
	m_Log->Log(a_Format, a_ArgList);
}





void cMCLogger::SetColor(eColorScheme a_Scheme)
{
	#ifdef _WIN32
		WORD Attrib = 0x07;  // by default, gray on black
		switch (a_Scheme)
		{
			case csGrayOnBlack:   Attrib = 0x07; break;
			case csYellowOnBlack: Attrib = 0x0e; break;
			case csRedOnBlack:    Attrib = 0x0c; break;
			case csBlackOnRed:    Attrib = 0xc0; break;
			default: ASSERT(!"Unhandled color scheme");
		}
		SetConsoleTextAttribute(g_Console, Attrib);
	#elif defined(LINUX) && !defined(ANDROID_NDK)
		switch (a_Scheme)
		{
			case csGrayOnBlack:   puts("\x1b[0m");    break;
			case csYellowOnBlack: puts("\x1b[33;1m"); break;
			case csRedOnBlack:    puts("\x1b[31;1m"); break;
			case csBlackOnRed:    puts("\x1b[7;31;1m"); break;
			default: ASSERT(!"Unhandled color scheme");
		}
	#endif
}





//////////////////////////////////////////////////////////////////////////
// Global functions
void LOG(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	cMCLogger::GetInstance()->Log( a_Format, argList );
	va_end(argList);
}

void LOGINFO(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	cMCLogger::GetInstance()->Info( a_Format, argList );
	va_end(argList);
}

void LOGWARN(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	cMCLogger::GetInstance()->Warn( a_Format, argList );
	va_end(argList);
}

void LOGERROR(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	cMCLogger::GetInstance()->Error( a_Format, argList );
	va_end(argList);
}




