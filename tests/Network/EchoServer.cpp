
// EchoServer.cpp

// Implements an Echo server using the LibEvent-based cNetwork API, as a test of that API

#include "Globals.h"
#include <iostream>
#include <string>
#include "OSSupport/Network.h"
#include "OSSupport/NetworkSingleton.h"





/** cTCPLink callbacks that echo everything they receive back to the remote peer. */
class cEchoLinkCallbacks:
	public cTCPLink::cCallbacks
{
	// cTCPLink::cCallbacks overrides:
	virtual void OnLinkCreated(cTCPLinkPtr a_Link) override
	{
		ASSERT(m_Link == nullptr);
		m_Link = a_Link;
	}


	virtual void OnReceivedData(const char * a_Data, size_t a_Size) override
	{
		ASSERT(m_Link != nullptr);

		// Echo the incoming data back to outgoing data:
		LOGD("%p (%s:%d): Data received (%u bytes), echoing back.", m_Link.get(), m_Link->GetRemoteIP().c_str(), m_Link->GetRemotePort(), static_cast<unsigned>(a_Size));
		m_Link->Send(a_Data, a_Size);
		LOGD("Echo queued");

		// Search for a Ctrl+Z, if found, drop the connection:
		for (size_t i = 0; i < a_Size; i++)
		{
			if (a_Data[i] == '\x1a')
			{
				m_Link->Close();
				m_Link.reset();
				return;
			}
		}
	}


	virtual void OnRemoteClosed(void) override
	{
		ASSERT(m_Link != nullptr);

		LOGD("%p (%s:%d): Remote has closed the connection.", m_Link.get(), m_Link->GetRemoteIP().c_str(), m_Link->GetRemotePort());
		m_Link.reset();
	}


	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
	{
		ASSERT(m_Link != nullptr);

		LOGD("%p (%s:%d): Error %d in the cEchoLinkCallbacks: %s", m_Link.get(), m_Link->GetRemoteIP().c_str(), m_Link->GetRemotePort(), a_ErrorCode, a_ErrorMsg.c_str());
		m_Link.reset();
	}

	/** The link attached to this callbacks instance. */
	cTCPLinkPtr m_Link;
};





class cEchoServerCallbacks:
	public cNetwork::cListenCallbacks
{
	virtual cTCPLink::cCallbacksPtr OnIncomingConnection(const AString & a_RemoteIPAddress, UInt16 a_RemotePort)
	{
		LOGD("New incoming connection(%s:%d).", a_RemoteIPAddress.c_str(), a_RemotePort);
		return std::make_shared<cEchoLinkCallbacks>();
	}

	virtual void OnAccepted(cTCPLink & a_Link) override
	{
		LOGD("New client accepted (%s:%d), sending welcome message.", a_Link.GetRemoteIP().c_str(), a_Link.GetRemotePort());
		// Send a welcome message to each connecting client:
		a_Link.Send("Welcome to the simple echo server.\r\n");
		LOGD("Welcome message queued.");
	}

	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
	{
		LOGWARNING("An error occured while listening for connections: %d (%s).", a_ErrorCode, a_ErrorMsg.c_str());
	}
};





void DoTest(void)
{
	LOGD("EchoServer: starting up");
	cServerHandlePtr Server = cNetwork::Listen(9876, std::make_shared<cEchoServerCallbacks>());
	if (!Server->IsListening())
	{
		LOGWARNING("Cannot listen on port 9876");
		abort();
	}
	ASSERT(Server->IsListening());

	// Wait for the user to terminate the server:
	printf("Press enter to close the server.\n");
	AString line;
	std::getline(std::cin, line);

	// Close the server and all its active connections:
	LOG("Server terminating.");
	Server->Close();
	ASSERT(!Server->IsListening());
	Server.reset();
	LOGD("Server has been closed.");
}





int main()
{
	DoTest();

	printf("Press enter to exit test.\n");
	AString line;
	std::getline(std::cin, line);
	cNetworkSingleton::Get().Terminate();

	LOG("Network test finished.");
	return 0;
}




