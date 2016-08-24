
// StackTrace.cpp

// Implements the functions to print current stack traces

#include "Globals.h"
#include "StackTrace.h"
#if defined(_WIN32)
	#include "../StackWalker.h"
#elif defined(__linux__)
	#include "../../lib/backward-cpp/backward.hpp"
#elif defined(__GLIBC__)
	#include <execinfo.h>
	#include <unistd.h>
#endif





void PrintStackTrace(void)
{
#if defined(_WIN32)
	// Reuse the StackWalker from the LeakFinder project already bound to MCS
	// Define a subclass of the StackWalker that outputs everything to stdout
	class PrintingStackWalker :
		public StackWalker
	{
		virtual void OnOutput(LPCSTR szText) override
		{
			puts(szText);
		}
	} sw;
	sw.ShowCallstack();
#elif defined(__linux__)
	// Use bacward-cpp (https://github.com/bombela/backward-cpp)
	backward::StackTrace st;
	st.load_here(32);
	backward::Printer p;
#if defined(_DEBUG)
	p.snippet = true;
#endif
	p.color = true;
	p.address = true;
	p.print(st, stderr);
#elif defined(__GLIBC__)
	// Use the backtrace() function to get and output the stackTrace:
	// Code adapted from https://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
	void * stackTrace[30];
	auto numItems = backtrace(stackTrace, ARRAYCOUNT(stackTrace));
	backtrace_symbols_fd(stackTrace, numItems, STDERR_FILENO);
#endif
}
