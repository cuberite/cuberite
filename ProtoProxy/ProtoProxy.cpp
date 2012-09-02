
// ProtoProxy.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include "Server.h"





int main(int argc, char ** argv)
{
	cServer Server;
	int res = Server.Init(25565, 25564);
	if (res != 0)
	{
		printf("Server initialization failed: %d", res);
		return res;
	}
	
	Server.Run();
	
	return 0;
}




