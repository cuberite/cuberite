
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "main.h"
#include "BuildInfo.h"
#include "Logger.h"
#include "MemorySettingsRepository.h"
#include "OSSupport/NetworkSingleton.h"
#include "OSSupport/MiniDumpWriter.h"
#include "OSSupport/StartAsService.h"
#include "Root.h"
#include "tclap/CmdLine.h"

#include <csignal>
#include <cstdlib>





bool g_ShouldLogCommIn;
bool g_ShouldLogCommOut;
bool g_RunAsService;

/** Global that registers itself as a last chance exception handler to write a minidump on crash. */
static MiniDumpWriter g_MiniDumpWriter;





// Because SIG_DFL or SIG_IGN could be NULL instead of nullptr, we need to disable the Clang warning here
#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wunknown-warning-option"
	#pragma clang diagnostic ignored "-Wunknown-pragmas"
	#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif  // __clang__

static void NonCtrlHandler(int a_Signal)
{
	LOGD("Terminate event raised from std::signal");

	switch (a_Signal)
	{
		case SIGSEGV:
		{
			PrintStackTrace();

			LOGERROR(
				"Failure report: \n\n"
				"  :(   | Cuberite has encountered an error and needs to close\n"
				"       | SIGSEGV: Segmentation fault\n"
				"       |\n"
#ifdef BUILD_ID
				"       | Cuberite " BUILD_SERIES_NAME " (id: " BUILD_ID ")\n"
				"       | from commit " BUILD_COMMIT_ID "\n"
#endif
			);

			std::signal(SIGSEGV, SIG_DFL);
			return;
		}
		case SIGABRT:
#ifdef SIGABRT_COMPAT
		case SIGABRT_COMPAT:
#endif
		{
			PrintStackTrace();

			LOGERROR(
				"Failure report: \n\n"
				"  :(   | Cuberite has encountered an error and needs to close\n"
				"       | SIGABRT: Server self-terminated due to an internal fault\n"
				"       |\n"
#ifdef BUILD_ID
				"       | Cuberite " BUILD_SERIES_NAME " (id: " BUILD_ID ")\n"
				"       | from commit " BUILD_COMMIT_ID "\n"
#endif
			);

			std::signal(SIGSEGV, SIG_DFL);
			return;
		}
		case SIGINT:
		case SIGTERM:
		{
			// Server is shutting down, wait for it...
			cRoot::Get()->Stop();
			return;
		}
#ifdef SIGPIPE
		case SIGPIPE:
		{
			// Ignore (PR #2487)
			return;
		}
#endif
	}
}

#ifdef __clang__
	#pragma clang diagnostic pop
#endif  // __clang__





#ifdef _WIN32
// Handle CTRL events in windows, including console window close
static BOOL CtrlHandler(DWORD fdwCtrlType)
{
	cRoot::Get()->Stop();
	LOGD("Terminate event raised from the Windows CtrlHandler");

	// Delay as much as possible to try to get the server to shut down cleanly - 10 seconds given by Windows
	std::this_thread::sleep_for(std::chrono::seconds(10));

	// Returning from main() automatically aborts this handler thread

	return TRUE;
}
#endif





////////////////////////////////////////////////////////////////////////////////
// ParseArguments - Read the startup arguments and store into a settings object

static void ParseArguments(int argc, char ** argv, cMemorySettingsRepository & repo)
{
	// Parse the comand line args:
	TCLAP::CmdLine cmd("Cuberite");
	TCLAP::ValueArg<int> slotsArg    ("s", "max-players",         "Maximum number of slots for the server to use, overrides setting in setting.ini", false, -1, "number", cmd);
	TCLAP::ValueArg<AString> confArg ("c", "config-file",         "Config file to use", false, "settings.ini", "string", cmd);
	TCLAP::MultiArg<int> portsArg    ("p", "port",                "The port number the server should listen to", false, "port", cmd);
	TCLAP::SwitchArg commLogArg      ("",  "log-comm",            "Log server client communications to file", cmd);
	TCLAP::SwitchArg commLogInArg    ("",  "log-comm-in",         "Log inbound server client communications to file", cmd);
	TCLAP::SwitchArg commLogOutArg   ("",  "log-comm-out",        "Log outbound server client communications to file", cmd);
	TCLAP::SwitchArg crashDumpFull   ("",  "crash-dump-full",     "Crashdumps created by the server will contain full server memory", cmd);
	TCLAP::SwitchArg crashDumpGlobals("",  "crash-dump-globals",  "Crashdumps created by the server will contain the global variables' values", cmd);
	TCLAP::SwitchArg noBufArg        ("",  "no-output-buffering", "Disable output buffering", cmd);
	TCLAP::SwitchArg noFileLogArg    ("",  "no-log-file",         "Disable logging to file", cmd);
	TCLAP::SwitchArg runAsServiceArg ("d", "service",             "Run as a service on Windows, or daemon on UNIX like systems", cmd);
	cmd.parse(argc, argv);

	// Copy the parsed args' values into a settings repository:
	if (confArg.isSet())
	{
		AString conf_file = confArg.getValue();
		repo.AddValue("Server", "ConfigFile", conf_file);
	}
	if (slotsArg.isSet())
	{
		int slots = slotsArg.getValue();
		repo.AddValue("Server", "MaxPlayers", static_cast<Int64>(slots));
	}
	if (portsArg.isSet())
	{
		for (auto port: portsArg.getValue())
		{
			repo.AddValue("Server", "Ports", std::to_string(port));
		}
	}
	if (noFileLogArg.getValue())
	{
		repo.AddValue("Server", "DisableLogFile", true);
	}
	if (commLogArg.getValue())
	{
		g_ShouldLogCommIn = true;
		g_ShouldLogCommOut = true;
	}
	else
	{
		g_ShouldLogCommIn = commLogInArg.getValue();
		g_ShouldLogCommOut = commLogOutArg.getValue();
	}
	if (noBufArg.getValue())
	{
		setvbuf(stdout, nullptr, _IONBF, 0);
	}
	repo.SetReadOnly();

	if (runAsServiceArg.getValue())
	{
		g_RunAsService = true;
	}

	// Apply the CrashDump flags for platforms that support them:
	if (crashDumpGlobals.getValue())
	{
		g_MiniDumpWriter.AddDumpFlags(MiniDumpFlags::WithDataSegments);
	}
	if (crashDumpFull.getValue())
	{
		g_MiniDumpWriter.AddDumpFlags(MiniDumpFlags::WithFullMemory);
	}
}





////////////////////////////////////////////////////////////////////////////////
// UniversalMain - Main startup logic for both standard running and as a service

static int UniversalMain(int argc, char * argv[], bool RunningAsService)
{
	// Initialize logging subsystem:
	cLogger::InitiateMultithreading();

	struct NetworkRAII
	{
		NetworkRAII()
		{
			// Initialize LibEvent:
			cNetworkSingleton::Get().Initialise();
		}

		~NetworkRAII()
		{
			// Shutdown all of LibEvent:
			cNetworkSingleton::Get().Terminate();
		}
	};

	try
	{
		// Make sure g_RunAsService is set correctly before checking it's value
		cMemorySettingsRepository Settings;
		ParseArguments(argc, argv, Settings);

		// Attempt to run as a service
		if (!RunningAsService && g_RunAsService)
		{
			// This will either fork or call UniversalMain again:
			if (cStartAsService::MakeIntoService<&UniversalMain>())
			{
				return EXIT_SUCCESS;
			}
		}

		while (true)
		{
			NetworkRAII LibEvent;
			cRoot Root;

			if (!Root.Run(Settings))
			{
				break;
			}
		}

		return EXIT_SUCCESS;
	}
	catch (const fmt::format_error & Oops)
	{
		std::cerr << "Formatting exception: " << Oops.what() << '\n';
	}
	catch (const TCLAP::ArgException & Oops)
	{
		std::cerr << fmt::sprintf("Error reading command line {} for argument {}\n", Oops.error(), Oops.argId());
	}
	catch (const std::exception & Oops)
	{
		std::cerr << "Standard exception: " << Oops.what() << '\n';
	}
	catch (...)
	{
		std::cerr << "Unknown exception!\n";
	}

	return EXIT_FAILURE;
}





int main(int argc, char ** argv)
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// _X: The simple built-in CRT leak finder - simply break when allocating the Nth block ({N} is listed in the leak output)
	// Only useful when the leak is in the same sequence all the time
	// _CrtSetBreakAlloc(85950);

#endif  // _DEBUG && _MSC_VER

	std::signal(SIGSEGV, NonCtrlHandler);
	std::signal(SIGTERM, NonCtrlHandler);
	std::signal(SIGINT,  NonCtrlHandler);
	std::signal(SIGABRT, NonCtrlHandler);
#ifdef SIGABRT_COMPAT
	std::signal(SIGABRT_COMPAT, NonCtrlHandler);
#endif
#ifdef SIGPIPE
	std::signal(SIGPIPE, SIG_IGN);
#endif

#ifdef _WIN32
	VERIFY(SetConsoleCtrlHandler(reinterpret_cast<PHANDLER_ROUTINE>(CtrlHandler), TRUE) == TRUE);
#endif

	return UniversalMain(argc, argv, false);
}
