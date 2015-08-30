
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

	int ListenPort  = (argc > 1) ? atoi(argv[1]) : 25564;
	int ConnectPort = (argc > 2) ? atoi(argv[2]) : 25565;
	printf("Initializing ProtoProxy. Listen port %d, connect port %d.\n", ListenPort, ConnectPort);
	cServer Server;
	int res = Server.Init(ListenPort, ConnectPort);
	if (res != 0)
	{
		printf("Server initialization failed: %d", res);
		return res;
	}
	
	Server.Run();
	
	return 0;
}




