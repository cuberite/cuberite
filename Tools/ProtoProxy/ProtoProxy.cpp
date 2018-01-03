
// ProtoProxy.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include "Server.h"
#include "../../src/Logger.h"
#include "../../src/LoggerListeners.h"





int main(int argc, char ** argv)
{
	// Initialize logging subsystem:
	auto consoleLogListener = MakeConsoleListener(false);
	auto consoleAttachment = cLogger::GetInstance().AttachListener(std::move(consoleLogListener));
	auto fileLogListenerRet = MakeFileListener();
	if (!fileLogListenerRet.first)
	{
		LOGERROR("Failed to open log file, aborting");
		return EXIT_FAILURE;
	}
	auto fileAttachment = cLogger::GetInstance().AttachListener(std::move(fileLogListenerRet.second));

	cLogger::InitiateMultithreading();

	UInt16 ListenPort = 25564;
	UInt16 ConnectPort = 25565;
	if (argc > 1)
	{
		if (!StringToInteger(argv[1], ListenPort))
		{
			LOGERROR("Invalid argument 1, expected port number, got \"%s\". Aborting.", argv[1]);
			return 1;
		}
		if (argc > 2)
		{
			if (!StringToInteger(argv[2], ConnectPort))
			{
				LOGERROR("Invalid argument 2, expected port number, got \"%s\". Aborting.", argv[2]);
				return 2;
			}
		}
	}
	cServer Server;
	int res = Server.Init(ListenPort, ConnectPort);
	if (res != 0)
	{
		LOGERROR("Server initialization failed: %d", res);
		return res;
	}

	Server.Run();

	return 0;
}
