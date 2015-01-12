
// EchoServer.cpp

// Implements an Echo server using the LibEvent-based cNetwork API, as a test of that API

#include "Globals.h"
#include <iostream>
#include <string>
#include "OSSupport/Network.h"





class cEchoServerCallbacks:
	public cNetwork::cListenCallbacks
{
	virtual void OnAccepted(cTCPLink & a_Link) override
	{
		LOGD("New client accepted, sending welcome message.");
		// Send a welcome message to each connecting client:
		a_Link.Send("Welcome to the simple echo server.\r\n");
		LOGD("Welcome message queued.");
	}
};





/** cTCPLink callbacks that echo everything they receive back to the remote peer. */
class cEchoLinkCallbacks:
	public cTCPLink::cCallbacks
{
	virtual void OnReceivedData(cTCPLink & a_Link, const char * a_Data, size_t a_Size) override
	{
		// Echo the incoming data back to outgoing data:
		LOGD("%p: Data received (%u bytes), echoing back.", &a_Link, static_cast<unsigned>(a_Size));
		a_Link.Send(a_Data, a_Size);
		LOGD("Echo queued");
	}

	virtual void OnRemoteClosed(cTCPLink & a_Link) override
	{
		LOGD("%p: Remote has closed the connection.", &a_Link);
	}

	virtual void OnError(cTCPLink & a_Link, int a_ErrorCode) override
	{
		LOGD("%p: Error %d in the cEchoLinkCallbacks.", &a_Link, a_ErrorCode);
	}
};





int main()
{
	LOGD("EchoServer: starting up");
	cServerHandlePtr Server = cNetwork::Listen(9876, std::make_shared<cEchoServerCallbacks>(), std::make_shared<cEchoLinkCallbacks>());
	if (!Server->IsListening())
	{
		LOGWARNING("Cannot listen on port 9876");
		abort();
	}
	ASSERT(Server->IsListening());

	// Wait for the user to terminate the server:
	printf("Press enter to terminate the server.\n");
	AString line;
	std::getline(std::cin, line);

	// Close the server and all its active connections:
	LOG("Server terminating.");
	Server->Close();
	ASSERT(!Server->IsListening());

	LOG("Network test finished.");
	return 0;
}




