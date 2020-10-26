
#include "Globals.h"

#include "LoggerListeners.h"

#if defined(_WIN32)
	#include <io.h>  // Needed for _isatty(), not available on Linux
	#include <time.h>
#endif


#if defined(_WIN32) || defined (__linux) || defined (__APPLE__)
	class cColouredConsoleListener
		: public cLogger::cListener
	{
	protected:

		virtual void SetLogColour(eLogLevel a_LogLevel) = 0;
		virtual void SetDefaultLogColour() = 0;

		virtual void Log(std::string_view a_Message, eLogLevel a_LogLevel) override
		{
			SetLogColour(a_LogLevel);
			fwrite(a_Message.data(), sizeof(char), a_Message.size(), stdout);
			SetDefaultLogColour();
		}
	};
#endif





#ifdef _WIN32
	class cWindowsConsoleListener:
		public cColouredConsoleListener
	{
		using Super = cColouredConsoleListener;

	public:

		cWindowsConsoleListener(HANDLE a_Console, WORD a_DefaultConsoleAttrib):
			m_Console(a_Console),
			m_DefaultConsoleAttrib(a_DefaultConsoleAttrib)
		{
		}

		#ifdef _DEBUG
			virtual void Log(std::string_view a_Message, eLogLevel a_LogLevel) override
			{
				Super::Log(a_Message, a_LogLevel);
				// In a Windows Debug build, output the log to debug console as well:
				OutputDebugStringA(AString(a_Message).c_str());
			}
		#endif


		virtual void SetLogColour(eLogLevel a_LogLevel) override
		{
			// by default, gray on black
			WORD Attrib = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			switch (a_LogLevel)
			{
				case eLogLevel::Regular:
				{
					// Gray on black
					Attrib = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
					break;
				}
				case eLogLevel::Info:
				{
					// Yellow on black
					Attrib = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
					break;
				}
				case eLogLevel::Warning:
				{
					// Red on black
					Attrib = FOREGROUND_RED | FOREGROUND_INTENSITY;
					break;
				}
				case eLogLevel::Error:
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



#elif defined (__linux) || defined (__APPLE__)



	class cANSIConsoleListener
		: public cColouredConsoleListener
	{
	public:
		virtual void SetLogColour(eLogLevel a_LogLevel) override
		{
			switch (a_LogLevel)
			{
				case eLogLevel::Regular:
				{
					// Whatever the console default is
					printf("\x1b[0m");
					break;
				}
				case eLogLevel::Info:
				{
					// Yellow on black
					printf("\x1b[33;1m");
					break;
				}
				case eLogLevel::Warning:
				{
					// Red on black
					printf("\x1b[31;1m");
					break;
				}
				case eLogLevel::Error:
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
			fflush(stdout);
		}
	};

#endif





class cVanillaCPPConsoleListener
	: public cLogger::cListener
{
public:
	virtual void Log(std::string_view a_Message, eLogLevel a_LogLevel) override
	{
		switch (a_LogLevel)
		{
			case eLogLevel::Regular:
			{
				fputs("Log: ", stdout);
				break;
			}
			case eLogLevel::Info:
			{
				fputs("Info: ", stdout);
				break;
			}
			case eLogLevel::Warning:
			{
				fputs("Warning: ", stdout);
				break;
			}
			case eLogLevel::Error:
			{
				fputs("Error: ", stdout);
				break;
			}
		}
		fwrite(a_Message.data(), sizeof(char), a_Message.size(), stdout);
	}
};





// Listener for when stdout is closed, i.e. When running as a daemon.
class cNullConsoleListener
	: public cLogger::cListener
{
	virtual void Log(std::string_view a_Message, eLogLevel a_LogLevel) override
	{
	}
};





std::unique_ptr<cLogger::cListener> MakeConsoleListener(bool a_IsService)
{
	if (a_IsService)
	{
		return std::make_unique<cNullConsoleListener>();
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
			return std::make_unique<cWindowsConsoleListener>(Console, DefaultConsoleAttrib);
		}
		else
		{
			return std::make_unique<cVanillaCPPConsoleListener>();
		}
	#elif defined (__linux) || defined (__APPLE__)
		// TODO: lookup terminal in terminfo
		if (isatty(fileno(stdout)))
		{
			return std::make_unique<cANSIConsoleListener>();
		}
		else
		{
			return std::make_unique<cVanillaCPPConsoleListener>();
		}
	#else
		return std::make_unique<cVanillaCPPConsoleListener>();
	#endif
}





////////////////////////////////////////////////////////////////////////////////
// cFileListener:

class cFileListener
	: public cLogger::cListener
{
public:

	cFileListener(void) {}

	bool Open()
	{
		// Assume creation succeeds, as the API does not provide a way to tell if the folder exists.
		cFile::CreateFolder("logs");
		bool success = m_File.Open(
			Printf(
				"logs/LOG_%d.txt",
				std::chrono::duration_cast<std::chrono::duration<int, std::ratio<1>>>(
					std::chrono::system_clock::now().time_since_epoch()
				).count()
			),
			cFile::fmAppend
		);
		return success;
	}

	virtual void Log(std::string_view a_Message, eLogLevel a_LogLevel) override
	{
		std::string_view LogLevelPrefix = "Unkn ";
		bool ShouldFlush = false;
		switch (a_LogLevel)
		{
			case eLogLevel::Regular:
			{
				LogLevelPrefix = "     ";
				break;
			}
			case eLogLevel::Info:
			{
				LogLevelPrefix = "Info ";
				break;
			}
			case eLogLevel::Warning:
			{
				LogLevelPrefix = "Warn ";
				ShouldFlush = true;
				break;
			}
			case eLogLevel::Error:
			{
				LogLevelPrefix = "Err  ";
				ShouldFlush = true;
				break;
			}
		}
		m_File.Write(LogLevelPrefix);
		m_File.Write(a_Message);

		if (ShouldFlush)
		{
			m_File.Flush();
		}
	}

private:

	cFile m_File;
};





std::pair<bool, std::unique_ptr<cLogger::cListener>> MakeFileListener()
{
	auto listener = std::make_unique<cFileListener>();
	if (!listener->Open())
	{
		return {false, nullptr};
	}
	return {true, std::move(listener)};
}


