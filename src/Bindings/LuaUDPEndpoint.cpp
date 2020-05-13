
// LuaUDPEndpoint.cpp

// Implements the cLuaUDPEndpoint class representing a Lua wrapper for the cUDPEndpoint class and the callbacks it needs

#include "Globals.h"
#include "LuaUDPEndpoint.h"





cLuaUDPEndpoint::cLuaUDPEndpoint(cLuaState::cTableRefPtr && a_Callbacks):
	m_Callbacks(std::move(a_Callbacks))
{
}





cLuaUDPEndpoint::~cLuaUDPEndpoint()
{
	// If the endpoint is still open, close it:
	auto endpoint = m_Endpoint;
	if (endpoint != nullptr)
	{
		endpoint->Close();
	}

	Terminated();
}





bool cLuaUDPEndpoint::Open(UInt16 a_Port, cLuaUDPEndpointPtr a_Self)
{
	ASSERT(m_Self == nullptr);  // Must not be opened yet
	ASSERT(m_Endpoint == nullptr);

	m_Self = std::move(a_Self);
	m_Endpoint = cNetwork::CreateUDPEndpoint(a_Port, *this);
	return m_Endpoint->IsOpen();
}





bool cLuaUDPEndpoint::Send(const AString & a_Data, const AString & a_RemotePeer, UInt16 a_RemotePort)
{
	// Safely grab a copy of the endpoint:
	auto endpoint = m_Endpoint;
	if (endpoint == nullptr)
	{
		return false;
	}

	// Send the data:
	return endpoint->Send(a_Data, a_RemotePeer, a_RemotePort);
}





UInt16 cLuaUDPEndpoint::GetPort(void) const
{
	// Safely grab a copy of the endpoint:
	auto endpoint = m_Endpoint;
	if (endpoint == nullptr)
	{
		return 0;
	}

	// Get the port:
	return endpoint->GetPort();
}





bool cLuaUDPEndpoint::IsOpen(void) const
{
	// Safely grab a copy of the endpoint:
	auto endpoint = m_Endpoint;
	if (endpoint == nullptr)
	{
		// No endpoint means that we're not open
		return false;
	}

	// Get the state:
	return endpoint->IsOpen();
}





void cLuaUDPEndpoint::Close(void)
{
	// If the endpoint is still open, close it:
	auto endpoint = m_Endpoint;
	if (endpoint != nullptr)
	{
		endpoint->Close();
		m_Endpoint.reset();
	}

	Terminated();
}





void cLuaUDPEndpoint::EnableBroadcasts(void)
{
	// Safely grab a copy of the endpoint:
	auto endpoint = m_Endpoint;
	if (endpoint != nullptr)
	{
		endpoint->EnableBroadcasts();
	}
}





void cLuaUDPEndpoint::Release(void)
{
	// Close the endpoint, if not already closed:
	Close();

	// Allow self to deallocate:
	m_Self.reset();
}





void cLuaUDPEndpoint::Terminated(void)
{
	// Disable the callbacks:
	m_Callbacks.reset();

	// If the endpoint is still open, close it:
	{
		auto endpoint = m_Endpoint;
		if (endpoint != nullptr)
		{
			endpoint->Close();
			m_Endpoint.reset();
		}
	}
}





void cLuaUDPEndpoint::OnReceivedData(const char * a_Data, size_t a_NumBytes, const AString & a_RemotePeer, UInt16 a_RemotePort)
{
	m_Callbacks->CallTableFn("OnReceivedData", this, AString(a_Data, a_NumBytes), a_RemotePeer, a_RemotePort);
}





void cLuaUDPEndpoint::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	// Notify the plugin:
	m_Callbacks->CallTableFn("OnError", a_ErrorCode, a_ErrorMsg);

	// Terminate all processing on the endpoint:
	Terminated();
}




