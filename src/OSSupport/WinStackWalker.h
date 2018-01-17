// WinStackWalker.h

// Declares the stack walking for Windows binaries

/*
LICENSE (https://www.opensource.org/licenses/bsd-license.php)

Copyright (c) 2005-2010, Jochen Kalmbach
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and / or other materials provided with the distribution.
Neither the name of Jochen Kalmbach nor the names of its contributors may be
used to endorse or promote products derived from this software without
specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once





#ifdef _WIN32  // This is Windows-only code





class StackWalkerInternal;  // forward
class WinStackWalker
{
public:
	enum StackWalkOptions
	{
		// No addition info will be retrived
		// (only the address is available)
		RetrieveNone = 0,

		// Try to get the symbol-name
		RetrieveSymbol = 1,

		// Try to get the line for this symbol
		RetrieveLine = 2,

		// Try to retrieve the module-infos
		RetrieveModuleInfo = 4,

		// Also retrieve the version for the DLL / EXE
		RetrieveFileVersion = 8,

		// Contains all the abouve
		RetrieveVerbose = 0xF,

		// Generate a "good" symbol-search-path
		SymBuildPath = 0x10,

		// Also use the public Microsoft-Symbol-Server
		SymUseSymSrv = 0x20,

		// Contains all the abouve "Sym"-options
		SymAll = 0x30,

		// Contains all options (default)
		OptionsAll = 0x3F
	};

	WinStackWalker(
		int options = OptionsAll,
		LPCSTR szSymPath = nullptr,
		DWORD dwProcessId = GetCurrentProcessId(),
		HANDLE hProcess = GetCurrentProcess()
	);

	WinStackWalker(DWORD dwProcessId, HANDLE hProcess);
	virtual ~WinStackWalker();

	typedef BOOL(__stdcall *PReadProcessMemoryRoutine)(
		HANDLE      hProcess,
		DWORD64     qwBaseAddress,
		PVOID       lpBuffer,
		DWORD       nSize,
		LPDWORD     lpNumberOfBytesRead,
		LPVOID      pUserData  // optional data, which was passed in "ShowCallstack"
	);

	BOOL LoadModules();

	BOOL ShowCallstack(
		HANDLE hThread = GetCurrentThread(),
		const CONTEXT *context = nullptr,
		PReadProcessMemoryRoutine readMemoryFunction = nullptr,
		LPVOID pUserData = nullptr  // optional to identify some data in the 'readMemoryFunction'-callback
	);

protected:
	enum
	{
		STACKWALK_MAX_NAMELEN = 1024,  ///< Max name length for found symbols
	};

protected:
	// Entry for each Callstack-Entry
	struct CallstackEntry
	{
		DWORD64 offset;  // if 0, we have no valid entry
		CHAR name[STACKWALK_MAX_NAMELEN];
		CHAR undName[STACKWALK_MAX_NAMELEN];
		CHAR undFullName[STACKWALK_MAX_NAMELEN];
		DWORD64 offsetFromSmybol;
		DWORD offsetFromLine;
		DWORD lineNumber;
		CHAR lineFileName[STACKWALK_MAX_NAMELEN];
		DWORD symType;
		LPCSTR symTypeString;
		CHAR moduleName[STACKWALK_MAX_NAMELEN];
		DWORD64 baseOfImage;
		CHAR loadedImageName[STACKWALK_MAX_NAMELEN];
	};

	enum CallstackEntryType { firstEntry, nextEntry, lastEntry };

	virtual void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName);
	virtual void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion);
	virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry);
	virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr);
	virtual void OnOutput(LPCSTR szText);

	StackWalkerInternal *m_sw;
	HANDLE m_hProcess;
	DWORD m_dwProcessId;
	BOOL m_modulesLoaded;
	LPSTR m_szSymPath;

	int m_options;
	int m_MaxRecursionCount;

	static BOOL __stdcall myReadProcMem(HANDLE hProcess, DWORD64 qwBaseAddress, PVOID lpBuffer, DWORD nSize, LPDWORD lpNumberOfBytesRead);

	friend StackWalkerInternal;
};




#define GET_CURRENT_CONTEXT(c, contextFlags) \
	do \
	{ \
		memset(&c, 0, sizeof(CONTEXT)); \
		c.ContextFlags = contextFlags; \
		RtlCaptureContext(&c); \
	} while (false);




#endif  // _WIN32
