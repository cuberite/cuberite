
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include <cstdarg>
#include <time.h>
#include "cLog.h"





cMCLogger* cMCLogger::s_MCLogger = 0;

cMCLogger* cMCLogger::GetInstance()
{
	return s_MCLogger;
}





cMCLogger::cMCLogger()
{
	AString FileName;
	Printf(FileName, "LOG_%d.txt", (int)time(0) );
	m_Log = new cLog(FileName);
	m_Log->Log("--- Started Log ---");

	s_MCLogger = this;
}





cMCLogger::cMCLogger( char* a_File )
{
	m_Log = new cLog( a_File );
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
			Log(a_Text, 0);
			break;
		case 1:
			Info(a_Text, 0);
			break;
		case 2:
			Warn(a_Text, 0);
			break;
		case 3:
			Error(a_Text, 0);
			break;
		default:
			Log(a_Text, 0);
			break;
	}
}





void cMCLogger::Log(const char* a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
	SetColor( 0x7 );	 // 0x7 is default grey color
	m_Log->Log( a_Format, a_ArgList );
	SetColor(0x07);  // revert color back
}





void cMCLogger::Info(const char* a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
// 	for( int i = 0; i < 16; i++)
// 	{
// 		for( int j = 0; j < 16; j++ )
// 		{
// 			SetConsoleTextAttribute( hConsole, i | (j<<4) );
// 			printf("0x%x", (i|j<<4));
// 		}
// 		printf("\n");
// 	}

	SetColor( 0xe );	 // 0xe is yellow
	m_Log->Log( a_Format, a_ArgList );
	SetColor(0x07);  // revert color back
}





void cMCLogger::Warn(const char* a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
	SetColor( 0xc );	 // 0xc is red
	m_Log->Log( a_Format, a_ArgList );
	SetColor(0x07);  // revert color back
}





void cMCLogger::Error(const char* a_Format, va_list a_ArgList)
{
	cCSLock Lock(m_CriticalSection);
	SetColor( 0xc0 );	// 0xc0 is red bg and black text
	m_Log->Log( a_Format, a_ArgList );
	SetColor(0x07);  // revert color back
}





void cMCLogger::SetColor( unsigned char a_Color )
{
#ifdef _WIN32
	HANDLE  hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute( hConsole, a_Color );
#else
	(void)a_Color;
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
