
// ProtoProxy.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include "Server.h"





int main(int argc, char ** argv)
{
	int ListenPort  = (argc > 1) ? atoi(argv[1]) : 25564;
	int ConnectPort = (argc > 2) ? atoi(argv[2]) : 25565;
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




