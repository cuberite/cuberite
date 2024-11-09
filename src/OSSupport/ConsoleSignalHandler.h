
// ConsoleSignalHandler.h

// Intercepts signals for graceful CTRL-C (and others) handling.

// This file MUST NOT be included from anywhere other than main.cpp.





#include <csignal>





// Because SIG_DFL or SIG_IGN could be NULL instead of nullptr, we need to disable the Clang warning here:
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

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
			cRoot::Stop();
			return;
		}
	}
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif





#ifdef _WIN32

/** Handle CTRL events in windows, including console window close. */
static BOOL CtrlHandler(DWORD fdwCtrlType)
{
	cRoot::Stop();
	LOGD("Terminate event raised from the Windows CtrlHandler");

	// Delay as much as possible to try to get the server to shut down cleanly - 10 seconds given by Windows:
	std::this_thread::sleep_for(std::chrono::seconds(10));

	// Returning from main() automatically aborts this handler thread.

	return TRUE;
}

#endif





namespace ConsoleSignalHandler
{
	static void Register()
	{
		std::signal(SIGSEGV, NonCtrlHandler);
		std::signal(SIGTERM, NonCtrlHandler);
		std::signal(SIGINT, NonCtrlHandler);
		std::signal(SIGABRT, NonCtrlHandler);
#ifdef SIGABRT_COMPAT
		std::signal(SIGABRT_COMPAT, NonCtrlHandler);
#endif
#ifdef SIGPIPE
		std::signal(SIGPIPE, SIG_IGN);  // Ignore (PR #2487).
#endif

#ifdef _WIN32
		SetConsoleCtrlHandler(reinterpret_cast<PHANDLER_ROUTINE>(CtrlHandler), TRUE);
#endif
	}
};
