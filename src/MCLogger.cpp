
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include <time.h>
#include "Log.h"





cMCLogger * cMCLogger::s_MCLogger = NULL;
bool g_ShouldColorOutput = false;

#ifdef _WIN32
	#include <io.h>  // Needed for _isatty(), not available on Linux
	
	HANDLE g_Console = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD g_DefaultConsoleAttrib = 0x07;
#elif defined (__linux) && !defined(ANDROID_NDK)
	#include <unistd.h>  // Needed for isatty() on Linux
#endif





cMCLogger * cMCLogger::GetInstance(void)
{
	return s_MCLogger;
}





cMCLogger::cMCLogger(void)
	: m_BeginLineUpdate(false)
{
	AString FileName;
	Printf(FileName, "LOG_%d.txt", (int)time(NULL));
	InitLog(FileName);
}





cMCLogger::cMCLogger(const AString & a_FileName)
	: m_BeginLineUpdate(false)
{
	InitLog(a_FileName);
}





cMCLogger::~cMCLogger()
{
	m_Log->Log("--- Stopped Log ---\n");
	delete m_Log;
	if (this == s_MCLogger)
	{
		s_MCLogger = NULL;
	}
}





void cMCLogger::InitLog(const AString & a_FileName)
{
	m_Log = new cLog(a_FileName);
	m_Log->Log("--- Started Log ---\n");

	s_MCLogger = this;

	#ifdef _WIN32
		// See whether we are writing to a console the default console attrib:
		g_ShouldColorOutput = (_isatty(_fileno(stdin)) != 0);
		if (g_ShouldColorOutput)
		{
			CONSOLE_SCREEN_BUFFER_INFO sbi;
			GetConsoleScreenBufferInfo(g_Console, &sbi);
			g_DefaultConsoleAttrib = sbi.wAttributes;
		}
	#elif defined (__linux) && !defined(ANDROID_NDK)
		g_ShouldColorOutput = isatty(fileno(stdout));
		// TODO: Check if the terminal supports colors, somehow?
	#endif
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





void cMCLogger::Log(const char * a_Format, va_list a_ArgList, bool a_ShouldReplaceLine)
{
	cCSLock Lock(m_CriticalSection);

	if (!m_BeginLineUpdate && a_ShouldReplaceLine)
	{
		a_ShouldReplaceLine = false; // Print a normal line first if this is the initial replace line
		m_BeginLineUpdate = true;
	}
	else if (m_BeginLineUpdate && !a_ShouldReplaceLine)
	{
		m_BeginLineUpdate = false;
	}

	if (a_ShouldReplaceLine)
	{
#ifdef _WIN32
		HANDLE Output = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(Output, &csbi);

		COORD Position = { 0, csbi.dwCursorPosition.Y - 1 }; // Move cursor up one line
		SetConsoleCursorPosition(Output, Position);

		SetColor(csRegular);
		if (!m_Log->LogReplaceLine(a_Format, a_ArgList))
		{
			m_BeginLineUpdate = false;
			puts("");
		}
		ResetColor();

		Position = { 0, csbi.dwCursorPosition.Y }; // Set cursor to original position
		SetConsoleCursorPosition(Output, Position);
#else // _WIN32
		fputs("\033[1A", stdout); // Move cursor up one line
		SetColor(csRegular);
		if (!m_Log->LogReplaceLine(a_Format, a_ArgList))
		{
			m_BeginLineUpdate = false;
			puts("");
		}
		ResetColor();
#endif
	}
	else
	{
		SetColor(csRegular);
		m_Log->Log(a_Format, a_ArgList);
		ResetColor();
		puts("");
	}
}





void cMCLogger::Info(const char * a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
	SetColor(csInfo);
	m_Log->Log(a_Format, a_ArgList);
	ResetColor();
	puts("");
}





void cMCLogger::Warn(const char * a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
	SetColor(csWarning);
	m_Log->Log(a_Format, a_ArgList);
	ResetColor();
	puts("");
}





void cMCLogger::Error(const char * a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
	SetColor(csError);
	m_Log->Log(a_Format, a_ArgList);
	ResetColor();
	puts("");
}





void cMCLogger::SetColor(eColorScheme a_Scheme)
{
	if (!g_ShouldColorOutput)
	{
		return;
	}
	#ifdef _WIN32
		WORD Attrib = 0x07;  // by default, gray on black
		switch (a_Scheme)
		{
			case csRegular: Attrib = 0x07; break;  // Gray on black
			case csInfo:    Attrib = 0x0e; break;  // Yellow on black
			case csWarning: Attrib = 0x0c; break;  // Read on black
			case csError:   Attrib = 0xc0; break;  // Black on red
			default: ASSERT(!"Unhandled color scheme");
		}
		SetConsoleTextAttribute(g_Console, Attrib);
	#elif (defined(__linux) || defined(__apple)) && !defined(ANDROID_NDK)
		switch (a_Scheme)
		{
			case csRegular: printf("\x1b[0m");         break;  // Whatever the console default is
			case csInfo:    printf("\x1b[33;1m");      break;  // Yellow on black
			case csWarning: printf("\x1b[31;1m");      break;  // Red on black
			case csError:   printf("\x1b[1;33;41;1m"); break;  // Yellow on red
			default: ASSERT(!"Unhandled color scheme");
		}
	#endif
}





void cMCLogger::ResetColor(void)
{
	if (!g_ShouldColorOutput)
	{
		return;
	}
	#ifdef _WIN32
		SetConsoleTextAttribute(g_Console, g_DefaultConsoleAttrib);
	#elif defined(__linux) && !defined(ANDROID_NDK)
		printf("\x1b[0m");
	#endif
}





//////////////////////////////////////////////////////////////////////////
// Global functions

void LOGREPLACELINE(const char* a_Format, ...)
{
	va_list argList;
	va_start(argList, a_Format);
	cMCLogger::GetInstance()->Log(a_Format, argList, true);
	va_end(argList);
}

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




