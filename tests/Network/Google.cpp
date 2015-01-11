
// Google.cpp

// Implements a HTTP download of the google's front page using the LibEvent-based cNetwork API

#include "Globals.h"
#include <thread>
#include "OSSupport/Event.h"
#include "OSSupport/Network.h"





/** Connect callbacks that send a HTTP GET request for google.com when connected. */
class cHTTPConnectCallbacks:
	public cNetwork::cConnectCallbacks
{
	cEvent & m_Event;
	virtual void OnSuccess(cTCPLink & a_Link) override
	{
		LOGD("Connected, sending HTTP GET");
		if (!a_Link.Send("GET / HTTP/1.0\r\nHost:google.com\r\n\r\n"))
		{
			LOGWARNING("Sending HTTP GET failed");
		}
		LOGD("HTTP GET queued.");
	}

	virtual void OnError(int a_ErrorCode) override
	{
		LOGD("Error while connecting HTTP: %d", a_ErrorCode);
		m_Event.Set();
	}

public:
	cHTTPConnectCallbacks(cEvent & a_Event):
		m_Event(a_Event)
	{
	}
};





/** cTCPLink callbacks that dump everything it received to the log. */
class cDumpCallbacks:
	public cTCPLink::cCallbacks
{
	cEvent & m_Event;

	virtual void OnReceivedData(cTCPLink & a_Link, const char * a_Data, size_t a_Size) override
	{
		// Log the incoming data size:
		AString Hex;
		CreateHexDump(Hex, a_Data, a_Size, 16);
		LOGD("Incoming data: %u bytes:\n%s", static_cast<unsigned>(a_Size), Hex.c_str());
	}

	virtual void OnRemoteClosed(cTCPLink & a_Link) override
	{
		LOGD("Remote has closed the connection.");
		m_Event.Set();
	}

	virtual void OnError(cTCPLink & a_Link, int a_ErrorCode) override
	{
		LOGD("Error in the cDumpCallbacks.");
		m_Event.Set();
	}

public:
	cDumpCallbacks(cEvent & a_Event):
		m_Event(a_Event)
	{
	}
};





int main()
{
	cEvent evtFinish;

	LOGD("Network test: Connecting to google.com:80, reading front page via HTTP.");
	if (!cNetwork::Connect("google.com", 80, std::make_shared<cHTTPConnectCallbacks>(evtFinish), std::make_shared<cDumpCallbacks>(evtFinish)))
	{
		LOGWARNING("Cannot queue connection to google.com");
		abort();
	}
	LOGD("Connect request has been queued.");
	
	evtFinish.Wait();
	LOGD("Network test finished");
}




