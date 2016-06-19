
// NameLookup.cpp

// Implements a DNS name lookup using the LibEvent-based cNetwork API

#include "Globals.h"
#include <thread>
#include "OSSupport/Event.h"
#include "OSSupport/Network.h"
#include "OSSupport/NetworkSingleton.h"





class cFinishLookupCallbacks:
	public cNetwork::cResolveNameCallbacks
{
	cEvent & m_Event;

	virtual void OnNameResolved(const AString & a_Name, const AString & a_IP) override
	{
		LOGD("%s resolves to IP %s", a_Name.c_str(), a_IP.c_str());
	}

	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override
	{
		LOGD("Error %d (%s) while performing lookup!", a_ErrorCode, a_ErrorMsg.c_str());
		exit(a_ErrorCode);
	}

	virtual void OnFinished(void) override
	{
		LOGD("Resolving finished.");
		m_Event.Set();
	}

public:
	cFinishLookupCallbacks(cEvent & a_Event):
		m_Event(a_Event)
	{
	}
};





static void DoTest(void)
{
	cEvent evtFinish;

	// Look up google.com (has multiple IP addresses):
	LOGD("Network test: Looking up google.com");
	if (!cNetwork::HostnameToIP("google.com", std::make_shared<cFinishLookupCallbacks>(evtFinish)))
	{
		LOGWARNING("Cannot resolve google.com to IP");
		abort();
	}
	LOGD("Name lookup has been successfully queued");
	evtFinish.Wait();
	LOGD("Lookup finished.");

	// Look up 8.8.8.8 (Google free DNS):
	LOGD("Network test: Looking up IP 8.8.8.8");
	if (!cNetwork::IPToHostName("8.8.8.8", std::make_shared<cFinishLookupCallbacks>(evtFinish)))
	{
		LOGWARNING("Cannot resolve 8.8.8.8 to name");
		abort();
	}
	LOGD("IP lookup has been successfully queued");
	evtFinish.Wait();
	LOGD("IP lookup finished.");
}





int main()
{
	LOGD("Initializing cNetwork...");
	cNetworkSingleton::Get().Initialise();

	LOGD("Running test...");
	DoTest();

	cNetworkSingleton::Get().Terminate();
	LOGD("Network test finished");
	return 0;
}




