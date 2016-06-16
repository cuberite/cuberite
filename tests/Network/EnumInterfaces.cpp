
// EnumInterfaces.cpp

// Implements the main app entrypoint for the EnumInterfaces network test
// Lists all network interfaces to the console

#include "Globals.h"
#include "OSSupport/Network.h"
#include "OSSupport/NetworkSingleton.h"





int main(int argc, char * argv[])
{
	// Initialize the cNetwork subsystem:
	LOGD("Initializing cNetwork...");
	cNetworkSingleton::Get().Initialise();

	// Enumerate all the addresses:
	printf("Enumerating all IP addresses...\n");
	auto IPs = cNetwork::EnumLocalIPAddresses();
	for (auto & ip: IPs)
	{
		printf("  %s\n", ip.c_str());
	}
	printf("Done.\n");

	// Terminate the cNetwork subsystem:
	cNetworkSingleton::Get().Terminate();

	return 0;
}




