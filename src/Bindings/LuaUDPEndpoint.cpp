
// LuaUDPEndpoint.cpp

// Implements the cLuaUDPEndpoint class representing a Lua wrapper for the cUDPEndpoint class and the callbacks it needs

#include "Globals.h"
#include "LuaUDPEndpoint.h"





cLuaUDPEndpoint::cLuaUDPEndpoint(cPluginLua & a_Plugin, int a_CallbacksTableStackPos):
	m_Plugin(a_Plugin),
	m_Callbacks(cPluginLua::cOperation(a_Plugin)(), a_CallbacksTableStackPos)
{
	// Warn if the callbacks aren't valid:
	if (!m_Callbacks.IsValid())
	{
		LOGWARNING("cLuaUDPEndpoint in plugin %s: callbacks could not be retrieved", m_Plugin.GetName().c_str());
		cPluginLua::cOperation Op(m_Plugin);
		Op().LogStackTrace();
	}
}





cLuaUDPEndpoint::~cLuaUDPEndpoint()
{
	// If the endpoint is still open, close it:
	cUDPEndpointPtr Endpoint = m_Endpoint;
	if (Endpoint != nullptr)
	{
		Endpoint->Close();
	}

	Terminated();
}





bool cLuaUDPEndpoint::Open(UInt16 a_Port, cLuaUDPEndpointPtr a_Self)
{
	ASSERT(m_Self == nullptr);  // Must not be opened yet
	ASSERT(m_Endpoint == nullptr);

	m_Self = a_Self;
	m_Endpoint = cNetwork::CreateUDPEndpoint(a_Port, *this);
	return m_Endpoint->IsOpen();
}





bool cLuaUDPEndpoint::Send(const AString & a_Data, const AString & a_RemotePeer, UInt16 a_RemotePort)
{
	// Safely grab a copy of the endpoint:
	cUDPEndpointPtr Endpoint = m_Endpoint;
	if (Endpoint == nullptr)
	{
		return false;
	}

	// Send the data:
	return Endpoint->Send(a_Data, a_RemotePeer, a_RemotePort);
}





UInt16 cLuaUDPEndpoint::GetPort(void) const
{
	// Safely grab a copy of the endpoint:
	cUDPEndpointPtr Endpoint = m_Endpoint;
	if (Endpoint == nullptr)
	{
		return 0;
	}

	// Get the port:
	return Endpoint->GetPort();
}





bool cLuaUDPEndpoint::IsOpen(void) const
{
	// Safely grab a copy of the endpoint:
	cUDPEndpointPtr Endpoint = m_Endpoint;
	if (Endpoint == nullptr)
	{
		// No endpoint means that we're not open
		return false;
	}

	// Get the state:
	return Endpoint->IsOpen();
}





void cLuaUDPEndpoint::Close(void)
{
	// If the endpoint is still open, close it:
	cUDPEndpointPtr Endpoint = m_Endpoint;
	if (Endpoint != nullptr)
	{
		Endpoint->Close();
		m_Endpoint.reset();
	}

	Terminated();
}





void cLuaUDPEndpoint::EnableBroadcasts(void)
{
	// Safely grab a copy of the endpoint:
	cUDPEndpointPtr Endpoint = m_Endpoint;
	if (Endpoint != nullptr)
	{
		Endpoint->EnableBroadcasts();
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
	if (m_Callbacks.IsValid())
	{
		m_Callbacks.UnRef();
	}

	// If the endpoint is still open, close it:
	{
		cUDPEndpointPtr Endpoint = m_Endpoint;
		if (Endpoint != nullptr)
		{
			Endpoint->Close();
			m_Endpoint.reset();
		}
	}
}





void cLuaUDPEndpoint::OnReceivedData(const char * a_Data, size_t a_NumBytes, const AString & a_RemotePeer, UInt16 a_RemotePort)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnReceivedData"), this, AString(a_Data, a_NumBytes), a_RemotePeer, a_RemotePort))
	{
		LOGINFO("cUDPEndpoint OnReceivedData callback failed in plugin %s.", m_Plugin.GetName().c_str());
	}
}





void cLuaUDPEndpoint::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnError"), a_ErrorCode, a_ErrorMsg))
	{
		LOGINFO("cUDPEndpoint OnError() callback failed in plugin %s; the endpoint error is %d (%s).",
			m_Plugin.GetName().c_str(), a_ErrorCode, a_ErrorMsg.c_str()
		);
	}

	Terminated();
}




