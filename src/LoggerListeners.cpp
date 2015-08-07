
#include "Globals.h"

#include "LoggerListeners.h"

#include <chrono>

#if defined(_WIN32)
	#include <io.h>  // Needed for _isatty(), not available on Linux
	#include <time.h>
#elif defined(__linux) && !defined(ANDROID_NDK)
	#include <unistd.h>  // Needed for isatty() on Linux
#elif defined(ANDROID_NDK)
	#include <android/log.h>
#endif


#if defined(_WIN32) || (defined (__linux) && !defined(ANDROID_NDK))
	class cColouredConsoleListener
		: public cLogger::cListener
	{
	protected:
	
		virtual void SetLogColour(cLogger::eLogLevel a_LogLevel) = 0;
		virtual void SetDefaultLogColour() = 0;
	
		virtual void Log(AString a_Message, cLogger::eLogLevel a_LogLevel) override
		{
			SetLogColour(a_LogLevel);
			fputs(a_Message.c_str(), stdout);
			SetDefaultLogColour();
		}
	};
#endif





#ifdef _WIN32
	class cWindowsConsoleListener
		: public cColouredConsoleListener
	{
		typedef cColouredConsoleListener super;
	public:
		cWindowsConsoleListener(HANDLE a_Console, WORD a_DefaultConsoleAttrib) :
			m_Console(a_Console),
			m_DefaultConsoleAttrib(a_DefaultConsoleAttrib)
		{
		}
		
		#ifdef _DEBUG
			virtual void Log(AString a_Message, cLogger::eLogLevel a_LogLevel) override
			{
				super::Log(a_Message, a_LogLevel);
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
				{
					// Gray on black
					Attrib = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
					break;
				}
				case cLogger::llInfo:
				{
					// Yellow on black
					Attrib = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
					break;
				}
				case cLogger::llWarning:
				{
					// Red on black
					Attrib = FOREGROUND_RED | FOREGROUND_INTENSITY;
					break;
				}
				case cLogger::llError:
				{
					// Black on red
					Attrib = BACKGROUND_RED | BACKGROUND_INTENSITY;
					break;
				}
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
				{
					// Whatever the console default is
					printf("\x1b[0m");
					break;
				}
				case cLogger::llInfo:
				{
					// Yellow on black
					printf("\x1b[33;1m");
					break;
				}
				case cLogger::llWarning:
				{
					// Red on black
					printf("\x1b[31;1m");
					break;
				}
				case cLogger::llError:
				{
					// Yellow on red
					printf("\x1b[1;33;41;1m");
					break;
				}
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
				{
					AndroidLogLevel = ANDROID_LOG_VERBOSE;
					break;
				}
				case cLogger::llInfo:
				{
					AndroidLogLevel = ANDROID_LOG_INFO;
					break;
				}
				case cLogger::llWarning:
				{
					AndroidLogLevel = ANDROID_LOG_WARNING;
					break;
				}
				case cLogger::llError:
				{
					AndroidLogLevel = ANDROID_LOG_ERROR;
					break;
				}
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
			{
				LogLevelString = "Log";
				break;
			}
			case cLogger::llInfo:
			{
				LogLevelString = "Info";
				break;
			}
			case cLogger::llWarning:
			{
				LogLevelString = "Warning";
				break;
			}
			case cLogger::llError:
			{
				LogLevelString = "Error";
				break;
			}
		}
		printf("%s: %s", LogLevelString.c_str(), a_Message.c_str());
	}
};





// Listener for when stdout is closed, i.e. When running as a daemon.
class cNullConsoleListener
	: public cLogger::cListener
{
	virtual void Log(AString a_Message, cLogger::eLogLevel a_LogLevel) override
	{
	}
};





cLogger::cListener * MakeConsoleListener(bool a_IsService)
{
	if (a_IsService)
	{
		return new cNullConsoleListener;
	}

	#ifdef _WIN32
		// See whether we are writing to a console the default console attrib:
		bool ShouldColorOutput = (_isatty(_fileno(stdin)) != 0);
		if (ShouldColorOutput)
		{
			CONSOLE_SCREEN_BUFFER_INFO sbi;
			HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(Console, &sbi);
			WORD DefaultConsoleAttrib = sbi.wAttributes;
			return new cWindowsConsoleListener(Console, DefaultConsoleAttrib);
		}
		else
		{
			return new cVanillaCPPConsoleListener;
		}
		
	#elif defined (__linux) && !defined(ANDROID_NDK)
		// TODO: lookup terminal in terminfo
		if (isatty(fileno(stdout)))
		{
			return new cLinuxConsoleListener();
		}
		else
		{
			return new cVanillaCPPConsoleListener();
		}
	#else
		return new cVanillaCPPConsoleListener();
	#endif
}





////////////////////////////////////////////////////////////////////////////////
// cFileListener:

cFileListener::cFileListener(void)
{
	cFile::CreateFolder(FILE_IO_PREFIX + AString("logs"));
	m_File.Open(
		FILE_IO_PREFIX + Printf(
			"logs/LOG_%d.txt",
			std::chrono::duration_cast<std::chrono::duration<int, std::ratio<1>>>(
				std::chrono::system_clock::now().time_since_epoch()
			).count()
		),
		cFile::fmAppend
	);
}





void cFileListener::Log(AString a_Message, cLogger::eLogLevel a_LogLevel)
{
	const char * LogLevelPrefix = "Unkn ";
	bool ShouldFlush = false;
	switch (a_LogLevel)
	{
		case cLogger::llRegular:
		{
			LogLevelPrefix = "     ";
			break;
		}
		case cLogger::llInfo:
		{
			LogLevelPrefix = "Info ";
			break;
		}
		case cLogger::llWarning:
		{
			LogLevelPrefix = "Warn ";
			ShouldFlush = true;
			break;
		}
		case cLogger::llError:
		{
			LogLevelPrefix = "Err  ";
			ShouldFlush = true;
			break;
		}
	}
	m_File.Printf("%s%s", LogLevelPrefix, a_Message.c_str());
	if (ShouldFlush)
	{
		m_File.Flush();
	}
}




