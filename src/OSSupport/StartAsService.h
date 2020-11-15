
#pragma once





#ifdef _WIN32

#include <csignal>





class cStartAsService
{
public:

	/** Make a Windows service. */
	template <auto UniversalMain>
	static bool MakeIntoService()
	{
		SERVICE_TABLE_ENTRY ServiceTable[] =
		{
			{ g_ServiceName, (LPSERVICE_MAIN_FUNCTION)serviceMain<UniversalMain> },
			{ nullptr, nullptr }
		};

		if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
		{
			throw std::system_error(GetLastError(), std::system_category());
		}

		return true;
	}

private:

	/** Set the internal status of the service */
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

	/** Handle stop events from the Service Control Manager */
	static void WINAPI serviceCtrlHandler(DWORD CtrlCode)
	{
		if (CtrlCode == SERVICE_CONTROL_STOP)
		{
			std::raise(SIGINT);
			serviceSetState(0, SERVICE_STOP_PENDING, 0);
		}
	}

	/* Startup logic for running as a service */
	template <auto MainFunction>
	static void WINAPI serviceMain(DWORD argc, TCHAR *argv[])
	{
		wchar_t applicationFilename[MAX_PATH];
		wchar_t applicationDirectory[MAX_PATH];

		// Get this binary's file path:
		if (GetModuleFileName(nullptr, applicationFilename, std::size(applicationFilename)) == 0)
		{
			serviceSetState(0, SERVICE_STOPPED, GetLastError());
			return;
		}

		const auto LastComponent = wcsrchr(applicationFilename, L'\\');
		if (LastComponent == nullptr)
		{
			serviceSetState(0, SERVICE_STOPPED, E_UNEXPECTED);
			return;
		}

		const auto LengthToLastComponent = LastComponent - applicationFilename;

		// Strip off the filename, keep only the path:
		wcsncpy(applicationDirectory, applicationFilename, LengthToLastComponent);
		applicationDirectory[LengthToLastComponent] = L'\0';  // Make sure new path is null terminated

		// Services are run by the SCM, and inherit its working directory - usually System32.
		// Set the working directory to the same location as the binary.
		if (SetCurrentDirectory(applicationDirectory) == FALSE)
		{
			serviceSetState(0, SERVICE_STOPPED, GetLastError());
			return;
		}


		g_StatusHandle = RegisterServiceCtrlHandler(g_ServiceName, serviceCtrlHandler);
		if (g_StatusHandle == nullptr)
		{
			OutputDebugStringA("RegisterServiceCtrlHandler() failed\n");
			serviceSetState(0, SERVICE_STOPPED, GetLastError());
			return;
		}

		serviceSetState(SERVICE_ACCEPT_STOP, SERVICE_RUNNING, 0);

		char MultibyteArgV0[MAX_PATH];
		char * MultibyteArgV[] = { MultibyteArgV0 };

		const auto OutputSize = std::size(MultibyteArgV0);
		const auto TranslateResult = wcstombs(MultibyteArgV0, argv[0], OutputSize);

		if (TranslateResult == static_cast<size_t>(-1))
		{
			// Translation failed entirely (!):
			MultibyteArgV0[0] = '\0';
		}
		else if (TranslateResult == OutputSize)
		{
			// Output too small:
			MultibyteArgV0[OutputSize - 1] = '\0';
		}

		const auto Result = MainFunction(1, MultibyteArgV, true);
		const auto Return = (Result == EXIT_SUCCESS) ? S_OK : E_FAIL;

		serviceSetState(0, SERVICE_STOPPED, Return);
	}

	static inline SERVICE_STATUS_HANDLE g_StatusHandle  = nullptr;
	static inline HANDLE                g_ServiceThread = INVALID_HANDLE_VALUE;
	static inline wchar_t               g_ServiceName[] = L"Cuberite";
};

#else

struct cStartAsService
{
	/** Make a UNIX daemon. */
	template <auto>
	static bool MakeIntoService()
	{
		pid_t pid = fork();

		// fork() returns a negative value on error.
		if (pid < 0)
		{
			throw std::system_error(errno, std::system_category());
		}

		// Check if we are the parent or child process. Parent stops here.
		if (pid > 0)
		{
			return true;
		}

		//  Child process now goes quiet, running in the background.
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		return false;
	}
};

#endif
