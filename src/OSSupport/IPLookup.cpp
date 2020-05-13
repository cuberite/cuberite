
// IPLookup.cpp

// Implements the cIPLookup class representing an IP-to-hostname lookup in progress.

#include "Globals.h"
#include "IPLookup.h"
#include <event2/util.h>
#include "NetworkSingleton.h"
#include "GetAddressInfoError.h"





////////////////////////////////////////////////////////////////////////////////
// cIPLookup:

cIPLookup::cIPLookup(const AString & a_IP, cNetwork::cResolveNameCallbacksPtr a_Callbacks):
	m_Callbacks(std::move(a_Callbacks)),
	m_IP(a_IP)
{
	ASSERT(m_Callbacks != nullptr);
}





void cIPLookup::Lookup(const AString & a_IP, cNetwork::cResolveNameCallbacksPtr a_Callbacks)
{
	cIPLookupPtr Lookup{ new cIPLookup(a_IP, std::move(a_Callbacks)) };  // Cannot use std::make_shared here, constructor is not accessible

	// Note the Lookup object is owned solely by this lambda which is destroyed after it runs
	cNetworkSingleton::Get().GetLookupThread().ScheduleLookup([=]()
	{
		sockaddr_storage sa;
		int salen = sizeof(sa);
		memset(&sa, 0, sizeof(sa));

		int ErrCode = evutil_parse_sockaddr_port(Lookup->m_IP.c_str(), reinterpret_cast<sockaddr *>(&sa), &salen);

		if (ErrCode != 0)
		{
			LOGD("Failed to parse IP address \"%s\".", Lookup->m_IP.c_str());
			Lookup->Callback(ErrCode, nullptr);
			return;
		}

		char Hostname[NI_MAXHOST];
		char ServInfo[NI_MAXSERV];

		ErrCode = getnameinfo(
			reinterpret_cast<sockaddr *>(&sa),
			static_cast<socklen_t>(salen),
			Hostname, sizeof(Hostname),
			ServInfo, sizeof(ServInfo),
			0
		);
		Lookup->Callback(ErrCode, Hostname);
	});
}





void cIPLookup::Callback(int a_Result, const char * a_Address)
{
	// Call the proper callback based on the event received:
	if ((a_Result != 0) || (a_Address == nullptr))
	{
		// An error has occurred, notify the error callback:
		m_Callbacks->OnError(a_Result, ErrorString(a_Result));
	}
	else
	{
		// Call the success handler:
		m_Callbacks->OnNameResolved(a_Address, m_IP);
		m_Callbacks->OnFinished();
	}
}





////////////////////////////////////////////////////////////////////////////////
// cNetwork API:

bool cNetwork::IPToHostName(
	const AString & a_IP,
	cNetwork::cResolveNameCallbacksPtr a_Callbacks
)
{
	cIPLookup::Lookup(a_IP, std::move(a_Callbacks));
	return true;
}




