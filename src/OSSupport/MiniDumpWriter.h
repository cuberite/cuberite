
// MiniDumpWriter.h

// 32-bit only:
// When the server crashes, create a "dump file" containing the callstack of each thread and some variables;
// let the user send us that crash file for analysis.

// This file MUST NOT be included from anywhere other than main.cpp.





/** Flags to control minidump contents on supported platforms. */
enum class MiniDumpFlags
{
	WithDataSegments,
	WithFullMemory
};





#if defined(_WIN32) && !defined(_WIN64) && defined(_MSC_VER)  // 32-bit Windows app compiled in MSVC

#include <DbgHelp.h>





using MiniDumpWriteDumpFunction = decltype(&MiniDumpWriteDump);

static HINSTANCE m_DbgHelp;
static MiniDumpWriteDumpFunction s_WriteMiniDump;  // The function in dbghlp DLL that creates dump files
static wchar_t s_DumpFileName[MAX_PATH];  // Filename of the dump file; hes to be created before the dump handler kicks in
static char s_ExceptionStack[128 * 1024];  // Substitute stack, just in case the handler kicks in because of "insufficient stack space"
static MINIDUMP_TYPE s_DumpFlags = MiniDumpNormal;  // By default dump only the stack and some helpers





/** This function gets called just before the "program executed an illegal instruction and will be terminated" or similar.
Its purpose is to create the crashdump using the dbghlp DLLs */
static LONG WINAPI LastChanceExceptionFilter(__in struct _EXCEPTION_POINTERS * a_ExceptionInfo)
{
	char * newStack = &s_ExceptionStack[sizeof(s_ExceptionStack) - 1];
	char * oldStack;

	// Use the substitute stack:
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
	HANDLE dumpFile = CreateFile(s_DumpFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	s_WriteMiniDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile, s_DumpFlags, (a_ExceptionInfo) ? &ExcInformation : nullptr, nullptr, nullptr);
	CloseHandle(dumpFile);

	// Revert to old stack:
	_asm
	{
		mov esp, oldStack
	}

	return 0;
}





namespace MiniDumpWriter
{
	static void Register()
	{
		// Magic code to produce dump-files on Windows if the server crashes:

		m_DbgHelp = LoadLibrary(L"DBGHELP.DLL");
		if (m_DbgHelp == INVALID_HANDLE_VALUE)
		{
			return;
		}

		s_WriteMiniDump = (MiniDumpWriteDumpFunction)GetProcAddress(m_DbgHelp, "MiniDumpWriteDump");
		if (s_WriteMiniDump != nullptr)
		{
			ASSERT(swprintf(s_DumpFileName, ARRAYCOUNT(s_DumpFileName), L"crash_mcs_%x.dmp", GetCurrentProcessId()) > 0);
			SetUnhandledExceptionFilter(LastChanceExceptionFilter);
		}

		// End of dump-file magic
	}

	static void AddDumpFlags(const MiniDumpFlags a_Flags)
	{
		switch (a_Flags)
		{
			case MiniDumpFlags::WithDataSegments:
			{
				s_DumpFlags = static_cast<MINIDUMP_TYPE>(s_DumpFlags | MINIDUMP_TYPE::MiniDumpWithDataSegs);
				break;
			}
			case MiniDumpFlags::WithFullMemory:
			{
				s_DumpFlags = static_cast<MINIDUMP_TYPE>(s_DumpFlags | MINIDUMP_TYPE::MiniDumpWithFullMemory);
				break;
			}
		}
	}

	static void Unregister()
	{
		FreeLibrary(m_DbgHelp);
	}
};

#else

namespace MiniDumpWriter
{
	static void Register()
	{
	}

	static void AddDumpFlags(const MiniDumpFlags)
	{
	}

	static void Unregister()
	{
	}
};

#endif
