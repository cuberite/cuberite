
#include "Globals.h"

#include "LoggerListeners.h"

#if defined(_WIN32)
	#include <io.h>  // Needed for _isatty(), not available on Linux
#elif defined(__linux) && !defined(ANDROID_NDK)
	#include <unistd.h>  // Needed for isatty() on Linux
#elif defined(ANDROID_NDK)
	#include <android/log.h>
#endif


#if defined(_WIN32) || (defined (__linux) && !defined(ANDROID_NDK))
	class cColouredConsoleListener
		: public cLogger::cListener
	{
	
		virtual void SetLogColour(cLogger::eLogLevel a_LogLevel) = 0;
		virtual void SetDefaultLogColour() = 0;
	
		virtual void Log(AString a_Message, cLogger::eLogLevel a_LogLevel) override
		{
			SetLogColour(a_LogLevel);
			puts(a_Message.c_str());
			SetDefaultLogColour();
		}
	};
#endif

#ifdef _WIN32
	class cWindowsConsoleListener
		: public cColouredConsoleListener
	{
	public:
		cWindowsConsoleListener(HANDLE a_Console, WORD a_DefaultConsoleAttrib) :
			m_Console(a_Console),
			m_DefaultConsoleAttrib(a_DefaultConsoleAttrib)
		{
		}
		
		#ifdef DEBUG
			virtual void Log(AString a_Message, cLogger::eLogLevel a_LogLevel) override
			{
				cColouredConsoleListener::Log(a_Message, a_LogLevel);
				// In a Windows Debug build, output the log to debug console as well:
				OutputDebugStringA(a_Message.c_str());
			}
		#endif
			
	
		virtual void SetLogColour(cLogger::eLogLevel a_LogLevel) override
		{
			// by default, gray on black
			WORD Attrib = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  
			switch (a_LogLevel)
			{
				case cLogger::llRegular:
					// Gray on black 
					Attrib = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED; 
					break;  
				case cLogger::llInfo:
					// Yellow on black
				    Attrib = FOREGROUND_GREEN | ;
				    break;
				case cLogger::llWarning:
					// Red on black
					Attrib = FOREGROUND_RED | FOREGROUND_INTENSITY;
					break;  
				case cLogger::llError:
					// Black on red   
					Attrib = BACKGROUND_RED | BACKGROUND_INTENSITY;
					break;
			}
			SetConsoleTextAttribute(m_Console, Attrib);
		}
		virtual void SetDefaultLogColour() override
		{ 
			SetConsoleTextAttribute(m_Console, m_DefaultConsoleAttrib);
		}
	private:
		HANDLE m_Console;
		WORD m_DefaultConsoleAttrib;
	};
#elif defined (__linux) && !defined(ANDROID_NDK)
	class cLinuxConsoleListener
		: public cColouredConsoleListener
	{
	public:
		virtual void SetLogColour(cLogger::eLogLevel a_LogLevel) override
		{
			switch (a_LogLevel)
			{
				case cLogger::llRegular:
					// Whatever the console default is 
					printf("\x1b[0m");
					break;  
				case cLogger::llInfo:
					// Yellow on black 
					printf("\x1b[33;1m");
					break;
				case cLogger::llWarning:
					// Red on black
					printf("\x1b[31;1m");
					break;  
				case cLogger::llError:
					// Yellow on red
					printf("\x1b[1;33;41;1m");
					break;
			}
		}
		virtual void SetDefaultLogColour() override
		{
			// Whatever the console default is 
			printf("\x1b[0m");
		}
	};
#elif defined(ANDROID_NDK)
	class cAndroidConsoleListener
		: public cLogger::cListener
	{
	public:
		virtual void Log(AString a_Message, cLogger::eLogLevel a_LogLevel) override
		{
			android_LogPriority AndroidLogLevel;
			switch (a_LogLevel)
			{
				case cLogger::llRegular:
					AndroidLogLevel = ANDROID_LOG_VERBOSE;
					break;
				case cLogger::llInfo:
					AndroidLogLevel = ANDROID_LOG_INFO;
					break;
				case cLogger::llWarning:
					AndroidLogLevel = ANDROID_LOG_WARNING;
					break;
				case cLogger::llError:
					AndroidLogLevel = ANDROID_LOG_ERROR;
					break;
			}
			__android_log_print(AndroidLogLevel, "MCServer", "%s", a_Message.c_str());
		}
	};
#endif

class cVanillaCPPConsoleListener
	: public cLogger::cListener
{
public:
	virtual void Log(AString a_Message, cLogger::eLogLevel a_LogLevel) override
	{
		AString LogLevelString;
		switch (a_LogLevel)
		{
			case cLogger::llRegular:
				LogLevelString = "Log";
				break;
			case cLogger::llInfo:
				LogLevelString = "Info";
				break;
			case cLogger::llWarning:
				LogLevelString = "Warning";
				break;
			case cLogger::llError:
				LogLevelString = "Error";
				break;
		}
		printf("%s: %s", LogLevelString.c_str(), a_Message.c_str());
	}
};



cLogger::cListener * MakeConsoleListener()
{
	#ifdef _WIN32
		// See whether we are writing to a console the default console attrib:
		bool ShouldColorOutput = (_isatty(_fileno(stdin)) != 0);
		if (ShouldColorOutput)
		{
			CONSOLE_SCREEN_BUFFER_INFO sbi;
			HANDLE Console = getStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(Console, &sbi);
			WORD DefaultConsoleAttrib = sbi.wAttributes;
			return new cWindowsConsoleListener(Console, DefaultConsoleAttrib);
		} else {
			return new cVanillaCPPConsoleListener();
		}
		
	#elif defined (__linux) && !defined(ANDROID_NDK)
		// TODO: lookup terminal in terminfo
		if (isatty(fileno(stdout)))
		{
			return new cLinuxConsoleListener();
		} else {
			return new cVanillaCPPConsoleListener();
		}
	#else
		return new cVanillaCPPConsoleListener();
	#endif
}

cFileListener::cFileListener()
{
	cFile::CreateFolder(FILE_IO_PREFIX + AString("logs"));
	AString FileName;
	FileName = Printf("%s%sLOG_%d.txt", FILE_IO_PREFIX, "logs/", (int)time(NULL));
	m_File.Open(FileName, cFile::fmAppend);
}

void cFileListener::Log(AString a_Message, cLogger::eLogLevel a_LogLevel)
{
	AString LogLevelString;
	switch (a_LogLevel)
	{
		case cLogger::llRegular:
			LogLevelString = "Log";
			break;
		case cLogger::llInfo:
			LogLevelString = "Info";
			break;
		case cLogger::llWarning:
			LogLevelString = "Warning";
			break;
		case cLogger::llError:
			LogLevelString = "Error";
			break;
	}
	m_File.Printf("%s: %s", LogLevelString.c_str(), a_Message.c_str());
}
