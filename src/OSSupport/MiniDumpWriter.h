
#pragma once





/** Flags to control minidump contents on supported platforms. */
enum class MiniDumpFlags
{
	WithDataSegments,
	WithFullMemory
};





#if defined(_WIN32) && !defined(_WIN64) && defined(_MSC_VER)  // 32-bit Windows app compiled in MSVC

#include <dbghelp.h>





/** Windows 32-bit stuff:
When the server crashes, create a "dump file" containing the callstack of each thread and some variables;
let the user send us that crash file for analysis */
class MiniDumpWriter
{
	typedef BOOL(WINAPI *pMiniDumpWriteDump)(
		HANDLE hProcess,
		DWORD ProcessId,
		HANDLE hFile,
		MINIDUMP_TYPE DumpType,
		PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
		PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
		PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);

public:

	MiniDumpWriter()
	{
		// Magic code to produce dump-files on Windows if the server crashes:

		m_DbgHelp = LoadLibrary(L"DBGHELP.DLL");
		if (m_DbgHelp == INVALID_HANDLE_VALUE)
		{
			return;
		}

		s_WriteMiniDump = (pMiniDumpWriteDump)GetProcAddress(m_DbgHelp, "MiniDumpWriteDump");
		if (s_WriteMiniDump != nullptr)
		{
			ASSERT(swprintf(s_DumpFileName, ARRAYCOUNT(s_DumpFileName), L"crash_mcs_%x.dmp", GetCurrentProcessId()) > 0);
			SetUnhandledExceptionFilter(LastChanceExceptionFilter);
		}

		// End of dump-file magic
	}

	void AddDumpFlags(const MiniDumpFlags a_Flags)
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

	~MiniDumpWriter()
	{
		FreeLibrary(m_DbgHelp);
	}

private:

	/** This function gets called just before the "program executed an illegal instruction and will be terminated" or similar.
	Its purpose is to create the crashdump using the dbghlp DLLs */
	static LONG WINAPI LastChanceExceptionFilter(__in struct _EXCEPTION_POINTERS * a_ExceptionInfo)
	{
		char * newStack = &s_ExceptionStack[sizeof(s_ExceptionStack) - 1];
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

	HINSTANCE m_DbgHelp;

	static inline pMiniDumpWriteDump s_WriteMiniDump;  // The function in dbghlp DLL that creates dump files
	static inline wchar_t s_DumpFileName[MAX_PATH];  // Filename of the dump file; hes to be created before the dump handler kicks in
	static inline char s_ExceptionStack[128 * 1024];  // Substitute stack, just in case the handler kicks in because of "insufficient stack space"
	static inline MINIDUMP_TYPE s_DumpFlags = MiniDumpNormal;  // By default dump only the stack and some helpers
};

#else

struct MiniDumpWriter
{
	void AddDumpFlags(const MiniDumpFlags)
	{
	}
};

#endif
