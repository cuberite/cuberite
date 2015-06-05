
// ProtoProxy.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include "Server.h"
#include "../../src/Logger.h"
#include "../../src/LoggerListeners.h"





int main(int argc, char ** argv)
{
	// Initialize logging subsystem:
	cLogger::InitiateMultithreading();
	auto consoleLogListener = MakeConsoleListener(false);
	auto fileLogListener = new cFileListener();
	cLogger::GetInstance().AttachListener(consoleLogListener);
	cLogger::GetInstance().AttachListener(fileLogListener);

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




