
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Root.h"
#include "tclap/CmdLine.h"

#include <exception>
#include <csignal>
#include <stdlib.h>



#ifdef ANDROID
	// Workaround for Android NDK builds that do not support std::to_string
	namespace std
	{
		template <typename T>
		std::string to_string(T Value)
		{
			std::ostringstream TempStream;
			TempStream << Value;
			return TempStream.str();
		}
	}
#endif

#ifdef _MSC_VER
	#include <dbghelp.h>
#endif  // _MSC_VER

#include "OSSupport/NetworkSingleton.h"
#include "BuildInfo.h"
#include "Logger.h"

#include "MemorySettingsRepository.h"




// Forward declarations to satisfy Clang's -Wmissing-variable-declarations:
extern bool g_ShouldLogCommIn;
extern bool g_ShouldLogCommOut;





/** If something has told the server to stop; checked periodically in cRoot */
bool cRoot::m_TerminateEventRaised = false;

/** If set to true, the protocols will log each player's incoming (C->S) communication to a per-connection logfile */
bool g_ShouldLogCommIn;

/** If set to true, the protocols will log each player's outgoing (S->C) communication to a per-connection logfile */
bool g_ShouldLogCommOut;

/** If set to true, binary will attempt to run as a service on Windows */
bool cRoot::m_RunAsService = false;





#if defined(_WIN32)
	SERVICE_STATUS_HANDLE g_StatusHandle  = nullptr;
	HANDLE                g_ServiceThread = INVALID_HANDLE_VALUE;
	#define               SERVICE_NAME      L"CuberiteService"
#endif





#ifndef _DEBUG
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
	cRoot::Get()->QueueExecuteConsoleCommand("stop");

	switch (a_Signal)
	{
		case SIGSEGV:
		{
			std::signal(SIGSEGV, SIG_DFL);
			LOGERROR("  D:    | Cuberite has encountered an error and needs to close");
			LOGERROR("Details | SIGSEGV: Segmentation fault");
			#ifdef BUILD_ID
			LOGERROR("Cuberite " BUILD_SERIES_NAME " build id: " BUILD_ID);
			LOGERROR("from commit id: " BUILD_COMMIT_ID " built at: " BUILD_DATETIME);
			#endif
			PrintStackTrace();
			abort();
		}
		case SIGABRT:
		#ifdef SIGABRT_COMPAT
		case SIGABRT_COMPAT:
		#endif
		{
			std::signal(a_Signal, SIG_DFL);
			LOGERROR("  D:    | Cuberite has encountered an error and needs to close");
			LOGERROR("Details | SIGABRT: Server self-terminated due to an internal fault");
			#ifdef BUILD_ID
			LOGERROR("Cuberite " BUILD_SERIES_NAME " build id: " BUILD_ID);
			LOGERROR("from commit id: " BUILD_COMMIT_ID " built at: " BUILD_DATETIME);
			#endif
			PrintStackTrace();
			abort();
		}
		case SIGINT:
		case SIGTERM:
		{
			std::signal(a_Signal, SIG_IGN);  // Server is shutting down, wait for it...
			break;
		}
		default: break;
	}
}

#ifdef __clang__
	#pragma clang diagnostic pop
#endif  // __clang__
#endif  // _DEBUG





#if defined(_WIN32) && !defined(_WIN64) && defined(_MSC_VER)
////////////////////////////////////////////////////////////////////////////////
// Windows 32-bit stuff: when the server crashes, create a "dump file" containing the callstack of each thread and some variables; let the user send us that crash file for analysis

typedef BOOL  (WINAPI *pMiniDumpWriteDump)(
	HANDLE hProcess,
	DWORD ProcessId,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	PMINIDUMP_CALLBACK_INFORMATION CallbackParam
);

static pMiniDumpWriteDump g_WriteMiniDump;  // The function in dbghlp DLL that creates dump files

static wchar_t g_DumpFileName[MAX_PATH];  // Filename of the dump file; hes to be created before the dump handler kicks in
static char g_ExceptionStack[128 * 1024];  // Substitute stack, just in case the handler kicks in because of "insufficient stack space"
static MINIDUMP_TYPE g_DumpFlags = MiniDumpNormal;  // By default dump only the stack and some helpers





/** This function gets called just before the "program executed an illegal instruction and will be terminated" or similar.
Its purpose is to create the crashdump using the dbghlp DLLs
*/
static LONG WINAPI LastChanceExceptionFilter(__in struct _EXCEPTION_POINTERS * a_ExceptionInfo)
{
	char * newStack = &g_ExceptionStack[sizeof(g_ExceptionStack) - 1];
	char * oldStack;

	// Use the substitute stack:
	// This code is the reason why we don't support 64-bit (yet)
	_asm
	{
		mov oldStack, esp
		mov esp, newStack
	}

	MINIDUMP_EXCEPTION_INFORMATION  ExcInformation;
	ExcInformation.ThreadId = GetCurrentThreadId();
	ExcInformation.ExceptionPointers = a_ExceptionInfo;
	ExcInformation.ClientPointers = 0;

	// Write the dump file:
	HANDLE dumpFile = CreateFile(g_DumpFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	g_WriteMiniDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile, g_DumpFlags, (a_ExceptionInfo) ? &ExcInformation : nullptr, nullptr, nullptr);
	CloseHandle(dumpFile);

	// Print the stack trace for the basic debugging:
	PrintStackTrace();

	// Revert to old stack:
	_asm
	{
		mov esp, oldStack
	}

	return 0;
}

#endif  // _WIN32 && !_WIN64





#ifdef _WIN32
// Handle CTRL events in windows, including console window close
static BOOL CtrlHandler(DWORD fdwCtrlType)
{
	cRoot::Get()->QueueExecuteConsoleCommand("stop");
	LOGD("Terminate event raised from the Windows CtrlHandler");

	std::this_thread::sleep_for(std::chrono::seconds(10));  // Delay as much as possible to try to get the server to shut down cleanly - 10 seconds given by Windows
	// Returning from main() automatically aborts this handler thread

	return TRUE;
}
#endif





////////////////////////////////////////////////////////////////////////////////
// UniversalMain - Main startup logic for both standard running and as a service

static void UniversalMain(std::unique_ptr<cSettingsRepositoryInterface> a_OverridesRepo)
{
	// Initialize logging subsystem:
	cLogger::InitiateMultithreading();

	// Initialize LibEvent:
	cNetworkSingleton::Get().Initialise();

	try
	{
		cRoot Root;
		Root.Start(std::move(a_OverridesRepo));
	}
	catch (fmt::FormatError & f)
	{
		FLOGERROR("Formatting exception: {0}", f.what());
	}
	catch (std::exception & e)
	{
		LOGERROR("Standard exception: %s", e.what());
	}
	catch (...)
	{
		LOGERROR("Unknown exception!");
	}

	// Shutdown all of LibEvent:
	cNetworkSingleton::Get().Terminate();
}





#if defined(_WIN32)  // Windows service support.
////////////////////////////////////////////////////////////////////////////////
// serviceWorkerThread: Keep the service alive

static DWORD WINAPI serviceWorkerThread(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);

	while (!cRoot::m_TerminateEventRaised)
	{
		// Do the normal startup
		UniversalMain(cpp14::make_unique<cMemorySettingsRepository>());
	}

	return ERROR_SUCCESS;
}





////////////////////////////////////////////////////////////////////////////////
// serviceSetState: Set the internal status of the service

static void serviceSetState(DWORD acceptedControls, DWORD newState, DWORD exitCode)
{
	SERVICE_STATUS serviceStatus = {};
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwControlsAccepted = acceptedControls;
	serviceStatus.dwCurrentState = newState;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwServiceType = SERVICE_WIN32;
	serviceStatus.dwWaitHint = 0;
	serviceStatus.dwWin32ExitCode = exitCode;

	if (SetServiceStatus(g_StatusHandle, &serviceStatus) == FALSE)
	{
		LOGERROR("SetServiceStatus() failed\n");
	}
}




////////////////////////////////////////////////////////////////////////////////
// serviceCtrlHandler: Handle stop events from the Service Control Manager

static void WINAPI serviceCtrlHandler(DWORD CtrlCode)
{
	switch (CtrlCode)
	{
		case SERVICE_CONTROL_STOP:
		{
			cRoot::Get()->QueueExecuteConsoleCommand("stop");
			serviceSetState(0, SERVICE_STOP_PENDING, 0);
			break;
		}
		default:
		{
			break;
		}
	}
}




////////////////////////////////////////////////////////////////////////////////
// serviceMain: Startup logic for running as a service

static void WINAPI serviceMain(DWORD argc, TCHAR *argv[])
{
	wchar_t applicationFilename[MAX_PATH];
	wchar_t applicationDirectory[MAX_PATH];

	GetModuleFileName(nullptr, applicationFilename, sizeof(applicationFilename));  // This binary's file path.

	// Strip off the filename, keep only the path:
	wcsncpy_s(applicationDirectory, sizeof(applicationDirectory), applicationFilename, (wcsrchr(applicationFilename, '\\') - applicationFilename));
	applicationDirectory[wcslen(applicationDirectory)] = '\0';  // Make sure new path is null terminated

	// Services are run by the SCM, and inherit its working directory - usually System32.
	// Set the working directory to the same location as the binary.
	SetCurrentDirectory(applicationDirectory);

	g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, serviceCtrlHandler);
	if (g_StatusHandle == nullptr)
	{
		OutputDebugStringA("RegisterServiceCtrlHandler() failed\n");
		serviceSetState(0, SERVICE_STOPPED, GetLastError());
		return;
	}

	serviceSetState(SERVICE_ACCEPT_STOP, SERVICE_RUNNING, 0);

	DWORD ThreadID;
	g_ServiceThread = CreateThread(nullptr, 0, serviceWorkerThread, nullptr, 0, &ThreadID);
	if (g_ServiceThread == nullptr)
	{
		OutputDebugStringA("CreateThread() failed\n");
		serviceSetState(0, SERVICE_STOPPED, GetLastError());
		return;
	}
	WaitForSingleObject(g_ServiceThread, INFINITE);  // Wait here for a stop signal.

	CloseHandle(g_ServiceThread);

	serviceSetState(0, SERVICE_STOPPED, 0);
}
#endif  // Windows service support.





static std::unique_ptr<cMemorySettingsRepository> ParseArguments(int argc, char ** argv)
{
	try
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
		auto repo = cpp14::make_unique<cMemorySettingsRepository>();
		if (confArg.isSet())
		{
			AString conf_file = confArg.getValue();
			repo->AddValue("Server", "ConfigFile", conf_file);
		}
		if (slotsArg.isSet())
		{
			int slots = slotsArg.getValue();
			repo->AddValue("Server", "MaxPlayers", static_cast<Int64>(slots));
		}
		if (portsArg.isSet())
		{
			for (auto port: portsArg.getValue())
			{
				repo->AddValue("Server", "Ports", std::to_string(port));
			}
		}
		if (noFileLogArg.getValue())
		{
			repo->AddValue("Server", "DisableLogFile", true);
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
		repo->SetReadOnly();

		// Set the service flag directly to cRoot:
		if (runAsServiceArg.getValue())
		{
			cRoot::m_RunAsService = true;
		}

		// Apply the CrashDump flags for platforms that support them:
		#if defined(_WIN32) && !defined(_WIN64) && defined(_MSC_VER)  // 32-bit Windows app compiled in MSVC
			if (crashDumpGlobals.getValue())
			{
				g_DumpFlags = static_cast<MINIDUMP_TYPE>(g_DumpFlags | MiniDumpWithDataSegs);
			}
			if (crashDumpFull.getValue())
			{
				g_DumpFlags = static_cast<MINIDUMP_TYPE>(g_DumpFlags | MiniDumpWithFullMemory);
			}
		#endif  // 32-bit Windows app compiled in MSVC

		return repo;
	}
	catch (const TCLAP::ArgException & exc)
	{
		fmt::print("Error reading command line {0} for arg {1}", exc.error(), exc.argId());
		return cpp14::make_unique<cMemorySettingsRepository>();
	}
}





////////////////////////////////////////////////////////////////////////////////
// main:

int main(int argc, char ** argv)
{
	// Magic code to produce dump-files on Windows if the server crashes:
	#if defined(_WIN32) && !defined(_WIN64) && defined(_MSC_VER)  // 32-bit Windows app compiled in MSVC
		HINSTANCE hDbgHelp = LoadLibrary(L"DBGHELP.DLL");
		g_WriteMiniDump = (pMiniDumpWriteDump)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
		if (g_WriteMiniDump != nullptr)
		{
			_snwprintf_s(g_DumpFileName, ARRAYCOUNT(g_DumpFileName), _TRUNCATE, L"crash_mcs_%x.dmp", GetCurrentProcessId());
			SetUnhandledExceptionFilter(LastChanceExceptionFilter);
		}
	#endif  // 32-bit Windows app compiled in MSVC
	// End of dump-file magic


	#if defined(_DEBUG) && defined(_MSC_VER)
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		// _X: The simple built-in CRT leak finder - simply break when allocating the Nth block ({N} is listed in the leak output)
		// Only useful when the leak is in the same sequence all the time
		// _CrtSetBreakAlloc(85950);

	#endif  // _DEBUG && _MSC_VER

	#ifndef _DEBUG
		std::signal(SIGSEGV, NonCtrlHandler);
		std::signal(SIGTERM, NonCtrlHandler);
		std::signal(SIGINT,  NonCtrlHandler);
		std::signal(SIGABRT, NonCtrlHandler);
		#ifdef SIGABRT_COMPAT
			std::signal(SIGABRT_COMPAT, NonCtrlHandler);
		#endif  // SIGABRT_COMPAT
	#endif


	#ifdef __unix__
		std::signal(SIGPIPE, SIG_IGN);
	#endif

	#ifdef _WIN32
		if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE))
		{
			LOGERROR("Could not install the Windows CTRL handler!");
		}
	#endif

	// Make sure m_RunAsService is set correctly before checking it's value
	ParseArguments(argc, argv);

	// Attempt to run as a service
	if (cRoot::m_RunAsService)
	{
		#if defined(_WIN32)  // Windows service.
			SERVICE_TABLE_ENTRY ServiceTable[] =
			{
				{ SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)serviceMain },
				{ nullptr, nullptr }
			};

			if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
			{
				LOGERROR("Attempted, but failed, service startup.");
				return GetLastError();
			}
		#else  // UNIX daemon.
			pid_t pid = fork();

			// fork() returns a negative value on error.
			if (pid < 0)
			{
				LOGERROR("Could not fork process.");
				return EXIT_FAILURE;
			}

			// Check if we are the parent or child process. Parent stops here.
			if (pid > 0)
			{
				return EXIT_SUCCESS;
			}

			//  Child process now goes quiet, running in the background.
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);

			while (!cRoot::m_TerminateEventRaised)
			{
				UniversalMain(ParseArguments(argc, argv));
			}
		#endif
	}
	else
	{
		while (!cRoot::m_TerminateEventRaised)
		{
			// Not running as a service, do normal startup
			UniversalMain(ParseArguments(argc, argv));
		}
	}
	return EXIT_SUCCESS;
}
