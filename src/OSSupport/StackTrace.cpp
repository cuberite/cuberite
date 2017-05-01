
// StackTrace.cpp

// Implements the functions to print current stack traces

#include "Globals.h"
#include "StackTrace.h"
#ifdef _WIN32
	#include "../StackWalker.h"
#elif !defined(ANDROID)  // The Android NDK has no execinfo header
	#ifdef __GLIBC__
		#include <cxxabi.h>
		#include <execinfo.h>
		#include <regex>
	#endif
	#include <unistd.h>
#endif





void PrintStackTrace(void)
{
	#ifdef _WIN32
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
	#elif !defined(ANDROID)
		#ifdef __GLIBC__
			auto output = stderr;
			void * stackTrace[66];
			char * demangled = nullptr;
			size_t demangledLen = 0;
			int demangledStatus;
			std::regex lineSplitter ("^([^(]+)\\(([^+]+)\\+(?:[^)]+)\\) \\[([^\\]]+)\\]$");

			auto numItems = backtrace(stackTrace, ARRAYCOUNT(stackTrace));
			char** buffer = backtrace_symbols(stackTrace, numItems);

			for (auto frameId = (numItems - 1); frameId > 0; frameId--)
			{
				bool handled = false;
				std::cmatch m;
				if (std::regex_match (buffer[frameId], m, lineSplitter))
				{
					demangled = abi::__cxa_demangle(m[2].str().c_str(), demangled, &demangledLen, &demangledStatus) ;
					if (demangledStatus == 0)
					{
						fprintf(output, "[#%i] \"%.*s\", at %.*s, in %s\n", frameId, (int)(m[1].second - m[1].first), m[1].first, (int)(m[3].second - m[3].first), m[3].first, demangled);
						handled = true;
					}
				}
				if (!handled)
				{
					fprintf(output, "[#%i] %s\n", frameId, buffer[frameId]);
				}
			}
			free(demangled);
		#endif
	#endif
}
