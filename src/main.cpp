
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "main.h"
#include "BuildInfo.h"
#include "Logger.h"
#include "MemorySettingsRepository.h"
#include "Root.h"
#include "tclap/CmdLine.h"

#include "OSSupport/ConsoleSignalHandler.h"
#include "OSSupport/NetworkSingleton.h"
#include "OSSupport/MiniDumpWriter.h"
#include "OSSupport/SleepResolutionBooster.h"
#include "OSSupport/StartAsService.h"





bool g_ShouldLogCommIn;
bool g_ShouldLogCommOut;
bool g_RunAsService;





////////////////////////////////////////////////////////////////////////////////
// ParseArguments - Read the startup arguments and store into a settings object

static void ParseArguments(int argc, char ** argv, cMemorySettingsRepository & a_Settings)
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
		a_Settings.AddValue("Server", "ConfigFile", conf_file);
	}
	if (slotsArg.isSet())
	{
		int slots = slotsArg.getValue();
		a_Settings.AddValue("Server", "MaxPlayers", static_cast<Int64>(slots));
	}
	if (portsArg.isSet())
	{
		for (auto port: portsArg.getValue())
		{
			a_Settings.AddValue("Server", "Ports", std::to_string(port));
		}
	}
	if (noFileLogArg.getValue())
	{
		a_Settings.AddValue("Server", "DisableLogFile", true);
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
	a_Settings.SetReadOnly();

	if (runAsServiceArg.getValue())
	{
		g_RunAsService = true;
	}

	// Apply the CrashDump flags for platforms that support them:
	if (crashDumpGlobals.getValue())
	{
		MiniDumpWriter::AddDumpFlags(MiniDumpFlags::WithDataSegments);
	}
	if (crashDumpFull.getValue())
	{
		MiniDumpWriter::AddDumpFlags(MiniDumpFlags::WithFullMemory);
	}
}





////////////////////////////////////////////////////////////////////////////////
// UniversalMain - Main startup logic for both standard running and as a service

static int UniversalMain(int argc, char * argv[], const bool a_RunningAsService)
{
	const struct MiniDumpWriterRAII
	{
		MiniDumpWriterRAII()
		{
			// Registers a last chance exception handler to write a minidump on crash:
			MiniDumpWriter::Register();
		}

		~MiniDumpWriterRAII()
		{
			MiniDumpWriter::Unregister();
		}
	} MiniDumpWriter;

	const struct SleepResolutionBoosterRAII
	{
		SleepResolutionBoosterRAII()
		{
			// Boost timer resolution to keep TPS high:
			SleepResolutionBooster::Register();
		}

		~SleepResolutionBoosterRAII()
		{
			SleepResolutionBooster::Unregister();
		}
	} SleepResolutionBooster;

	// Register signal handlers, enabling graceful shutdown from the terminal:
	ConsoleSignalHandler::Register();

	// Initialize logging subsystem:
	cLogger::InitiateMultithreading();

	try
	{
		cMemorySettingsRepository Settings;
		ParseArguments(argc, argv, Settings);  // Make sure g_RunAsService is set correctly before checking its value.

		// Attempt to run as a service:
		if (g_RunAsService && !a_RunningAsService)
		{
			// This will either fork or call UniversalMain again:
			if (StartAsService::MakeIntoService<&UniversalMain>())
			{
				return EXIT_SUCCESS;
			}
		}

		while (true)
		{
			const struct NetworkRAII
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
			} LibEvent;

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

#endif  // !NDEBUG && _MSC_VER

	return UniversalMain(argc, argv, false);
}
