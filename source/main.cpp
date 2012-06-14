
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cRoot.h"

#include <exception> //std::exception
#include <csignal>   //std::signal
#include <stdlib.h>  //exit()

#ifdef _WIN32
	#include <dbghelp.h>
#endif  // _WIN32

#include "SquirrelBindings.h"
#if USE_SQUIRREL
	#pragma warning(push)
	#pragma warning(disable:4100;disable:4127;disable:4510;disable:4610;disable:4244;disable:4512) // Getting A LOT of these warnings from SqPlus
	#include <sqplus/sqplus.h>
	#pragma warning(pop)
#endif




/// If defined, a thorough leak finder will be used (debug MSVC only); leaks will be output to the Output window
#define ENABLE_LEAK_FINDER





#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	#define XML_LEAK_FINDER
	#pragma warning(push)
	#pragma warning(disable:4100)
	#include "LeakFinder.h"
	#pragma warning(pop)
#endif






void ShowCrashReport(int) 
{
	std::signal(SIGSEGV, SIG_DFL);

	printf("\n\nMCServer has crashed!\n");

	exit(-1);
}





#if defined(_WIN32) && !defined(_WIN64)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main:

int main( int argc, char **argv )
{
	(void)argc;
	(void)argv;
	
	#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	InitLeakFinder();
	#endif
	
	// Magic code to produce dump-files on Windows if the server crashes:
	#if defined(_WIN32) && !defined(_WIN64)
	HINSTANCE hDbgHelp = LoadLibrary("DBGHELP.DLL");
	g_WriteMiniDump = (pMiniDumpWriteDump)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	if (g_WriteMiniDump != NULL)
	{
		_snprintf_s(g_DumpFileName, ARRAYCOUNT(g_DumpFileName), _TRUNCATE, "crash_mcs_%x.dmp", GetCurrentProcessId());
		SetUnhandledExceptionFilter(LastChanceExceptionFilter);
		
		// Parse arguments for minidump flags:
		for (int i = 0; i < argc; i++)
		{
			if (stricmp(argv[i], "/cdg") == 0)
			{
				// Add globals to the dump
				g_DumpFlags = (MINIDUMP_TYPE)(g_DumpFlags | MiniDumpWithDataSegs);
			}
			else if (stricmp(argv[i], "/cdf") == 0)
			{
				// Add full memory to the dump (HUUUGE file)
				g_DumpFlags = (MINIDUMP_TYPE)(g_DumpFlags | MiniDumpWithFullMemory);
			}
		}  // for i - argv[]
	}
	#endif  // _WIN32 && !_WIN64
	// End of dump-file magic
	
	#if defined(_DEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	
	// _X: The simple built-in CRT leak finder - simply break when allocating the Nth block ({N} is listed in the leak output)
	// Only useful when the leak is in the same sequence all the time
	// _CrtSetBreakAlloc(85950);
	
	#endif  // _DEBUG && _MSC_VER

	#ifndef _DEBUG
	std::signal(SIGSEGV, ShowCrashReport);
	#endif

	// DEBUG: test the dumpfile creation:
	// *((int *)0) = 0;
	
	try
	{
		cRoot Root;	
		Root.Start();
	}
	catch( std::exception& e )
	{
		LOGERROR("Standard exception: %s", e.what() );
	}
	catch( ... )
	{
		LOGERROR("Unknown exception!");
	}

	#if USE_SQUIRREL
	SquirrelVM::Shutdown();
	#endif

	#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	DeinitLeakFinder();
	#endif
	
	return 0;
}




