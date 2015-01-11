
// NameLookup.cpp

// Implements a DNS name lookup using the LibEvent-based cNetwork API

#include "Globals.h"
#include <thread>
#include "OSSupport/Event.h"
#include "OSSupport/Network.h"





class cFinishLookupCallbacks:
	public cNetwork::cResolveNameCallbacks
{
	cEvent & m_Event;

	virtual void OnNameResolved(const AString & a_Name, const AString & a_IP) override
	{
		LOGD("%s resolves to IP %s", a_Name.c_str(), a_IP.c_str());
	}

	virtual void OnError(int a_ErrorCode) override
	{
		LOGD("Error %d while performing lookup!", a_ErrorCode);
		abort();
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





int main()
{
	cEvent evtFinish;

	LOGD("Network test: Looking up google.com");
	if (!cNetwork::HostnameToIP("google.com", std::make_shared<cFinishLookupCallbacks>(evtFinish)))
	{
		LOGWARNING("Cannot resolve google.com");
		abort();
	}
	LOGD("Name lookup has been successfully queued");
	
	evtFinish.Wait();
	LOGD("Network test finished");
	return 0;
}




