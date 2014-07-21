
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Root.h"

#include <exception>
#include <csignal>
#include <stdlib.h>

#ifdef _MSC_VER
	#include <dbghelp.h>
#endif  // _MSC_VER


bool cRoot::m_TerminateEventRaised = false;  // If something has told the server to stop; checked periodically in cRoot
static bool g_ServerTerminated = false;  // Set to true when the server terminates, so our CTRL handler can then tell the OS to close the console





/** If set to true, the protocols will log each player's incoming (C->S) communication to a per-connection logfile */
bool g_ShouldLogCommIn;

/** If set to true, the protocols will log each player's outgoing (S->C) communication to a per-connection logfile */
bool g_ShouldLogCommOut;





/// If defined, a thorough leak finder will be used (debug MSVC only); leaks will be output to the Output window
// _X 2014_02_20: Disabled for canon repo, it makes the debug version too slow in MSVC2013
// and we haven't had a memory leak for over a year anyway.
// #define ENABLE_LEAK_FINDER





#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	#pragma warning(push)
	#pragma warning(disable:4100)
	#include "LeakFinder.h"
	#pragma warning(pop)
#endif





void NonCtrlHandler(int a_Signal)
{
	LOGD("Terminate event raised from std::signal");
	cRoot::m_TerminateEventRaised = true;

	switch (a_Signal)
	{
		case SIGSEGV:
		{
			std::signal(SIGSEGV, SIG_DFL);
			LOGERROR("  D:    | MCServer has encountered an error and needs to close");
			LOGERROR("Details | SIGSEGV: Segmentation fault");
			abort();
		}
		case SIGABRT:
		#ifdef SIGABRT_COMPAT
		case SIGABRT_COMPAT:
		#endif
		{
			std::signal(a_Signal, SIG_DFL);
			LOGERROR("  D:    | MCServer has encountered an error and needs to close");
			LOGERROR("Details | SIGABRT: Server self-terminated due to an internal fault");
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

pMiniDumpWriteDump g_WriteMiniDump;  // The function in dbghlp DLL that creates dump files

char g_DumpFileName[MAX_PATH];  // Filename of the dump file; hes to be created before the dump handler kicks in
char g_ExceptionStack[128 * 1024];  // Substitute stack, just in case the handler kicks in because of "insufficient stack space"
MINIDUMP_TYPE g_DumpFlags = MiniDumpNormal;  // By default dump only the stack and some helpers





/** This function gets called just before the "program executed an illegal instruction and will be terminated" or similar.
Its purpose is to create the crashdump using the dbghlp DLLs
*/
LONG WINAPI LastChanceExceptionFilter(__in struct _EXCEPTION_POINTERS * a_ExceptionInfo)
{
	char * newStack = &g_ExceptionStack[sizeof(g_ExceptionStack)];
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
	HANDLE dumpFile = CreateFile(g_DumpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	g_WriteMiniDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile, g_DumpFlags, (a_ExceptionInfo) ? &ExcInformation : NULL, NULL, NULL);
	CloseHandle(dumpFile);

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
BOOL CtrlHandler(DWORD fdwCtrlType)
{
	cRoot::m_TerminateEventRaised = true;
	LOGD("Terminate event raised from the Windows CtrlHandler");

	if (fdwCtrlType == CTRL_CLOSE_EVENT)  // Console window closed via 'x' button, Windows will try to close immediately, therefore...
	{
		while (!g_ServerTerminated) { cSleep::MilliSleep(100); }  // Delay as much as possible to try to get the server to shut down cleanly
	}

	return TRUE;
}
#endif





////////////////////////////////////////////////////////////////////////////////
// main:

int main( int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);
	
	#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	InitLeakFinder();
	#endif
	
	// Magic code to produce dump-files on Windows if the server crashes:
	#if defined(_WIN32) && !defined(_WIN64) && defined(_MSC_VER)
	HINSTANCE hDbgHelp = LoadLibrary("DBGHELP.DLL");
	g_WriteMiniDump = (pMiniDumpWriteDump)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	if (g_WriteMiniDump != NULL)
	{
		_snprintf_s(g_DumpFileName, ARRAYCOUNT(g_DumpFileName), _TRUNCATE, "crash_mcs_%x.dmp", GetCurrentProcessId());
		SetUnhandledExceptionFilter(LastChanceExceptionFilter);
		
		// Parse arguments for minidump flags:
		for (int i = 0; i < argc; i++)
		{
			if (_stricmp(argv[i], "/cdg") == 0)
			{
				// Add globals to the dump
				g_DumpFlags = (MINIDUMP_TYPE)(g_DumpFlags | MiniDumpWithDataSegs);
			}
			else if (_stricmp(argv[i], "/cdf") == 0)
			{
				// Add full memory to the dump (HUUUGE file)
				g_DumpFlags = (MINIDUMP_TYPE)(g_DumpFlags | MiniDumpWithFullMemory);
			}
		}  // for i - argv[]
	}
	#endif  // _WIN32 && !_WIN64
	// End of dump-file magic

	#ifdef _WIN32
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE))
	{
		LOGERROR("Could not install the Windows CTRL handler!");
	}
	#endif
	
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

	// DEBUG: test the dumpfile creation:
	// *((int *)0) = 0;
	
	// Check if comm logging is to be enabled:
	for (int i = 0; i < argc; i++)
	{
		AString Arg(argv[i]);
		if (
			(NoCaseCompare(Arg, "/commlog") == 0) ||
			(NoCaseCompare(Arg, "/logcomm") == 0)
		)
		{
			g_ShouldLogCommIn = true;
			g_ShouldLogCommOut = true;
		}
		else if (
			(NoCaseCompare(Arg, "/commlogin") == 0) ||
			(NoCaseCompare(Arg, "/comminlog") == 0) ||
			(NoCaseCompare(Arg, "/logcommin") == 0)
		)
		{
			g_ShouldLogCommIn = true;
		}
		else if (
			(NoCaseCompare(Arg, "/commlogout") == 0) ||
			(NoCaseCompare(Arg, "/commoutlog") == 0) ||
			(NoCaseCompare(Arg, "/logcommout") == 0)
		)
		{
			g_ShouldLogCommOut = true;
		}
		else if (NoCaseCompare(Arg, "nooutbuf") == 0)
		{
			setvbuf(stdout, NULL, _IONBF, 0);
		}
	}  // for i - argv[]
	
	#if !defined(ANDROID_NDK)
	try
	#endif
	{
		cRoot Root;
		Root.Start();
	}
	#if !defined(ANDROID_NDK)
	catch (std::exception & e)
	{
		LOGERROR("Standard exception: %s", e.what());
	}
	catch (...)
	{
		LOGERROR("Unknown exception!");
	}
	#endif


	#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	DeinitLeakFinder();
	#endif

	g_ServerTerminated = true;

	return EXIT_SUCCESS;
}




