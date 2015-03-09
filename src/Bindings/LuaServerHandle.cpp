
// LuaServerHandle.cpp

// Implements the cLuaServerHandle class wrapping the cServerHandle functionality for Lua API

#include "Globals.h"
#include "LuaServerHandle.h"
#include "LuaTCPLink.h"
#include "tolua++/include/tolua++.h"





cLuaServerHandle::cLuaServerHandle(UInt16 a_Port, cPluginLua & a_Plugin, int a_CallbacksTableStackPos):
	m_Plugin(a_Plugin),
	m_Callbacks(a_Plugin.GetLuaState(), a_CallbacksTableStackPos),
	m_Port(a_Port)
{
}






cLuaServerHandle::~cLuaServerHandle()
{
	// If the server handle is still open, close it explicitly:
	Close();
}





void cLuaServerHandle::SetServerHandle(cServerHandlePtr a_ServerHandle, cLuaServerHandlePtr a_Self)
{
	ASSERT(m_ServerHandle == nullptr);  // The handle can be set only once

	m_ServerHandle = a_ServerHandle;
	m_Self = a_Self;
}





void cLuaServerHandle::Close(void)
{
	// Safely grab a copy of the server handle:
	cServerHandlePtr ServerHandle = m_ServerHandle;
	if (ServerHandle == nullptr)
	{
		return;
	}

	// Close the underlying server handle:
	ServerHandle->Close();

	// Close all connections at this server:
	cLuaTCPLinkPtrs Connections;
	{
		cCSLock Lock(m_CSConnections);
		std::swap(Connections, m_Connections);
	}
	for (auto & conn: Connections)
	{
		conn->Close();
	}
	Connections.clear();

	// Allow the internal server handle to be deallocated:
	m_ServerHandle.reset();
}





bool cLuaServerHandle::IsListening(void)
{
	// Safely grab a copy of the server handle:
	cServerHandlePtr ServerHandle = m_ServerHandle;
	if (ServerHandle == nullptr)
	{
		return false;
	}

	// Query the underlying server handle:
	return ServerHandle->IsListening();
}





void cLuaServerHandle::RemoveLink(cLuaTCPLink * a_Link)
{
	cCSLock Lock(m_CSConnections);
	for (auto itr = m_Connections.begin(), end = m_Connections.end(); itr != end; ++itr)
	{
		if (itr->get() == a_Link)
		{
			m_Connections.erase(itr);
			break;
		}
	}  // for itr - m_Connections[]
}





void cLuaServerHandle::Release(void)
{
	// Close the server, if it isn't closed yet:
	Close();

	// Allow self to deallocate:
	m_Self.reset();
}





cTCPLink::cCallbacksPtr cLuaServerHandle::OnIncomingConnection(const AString & a_RemoteIPAddress, UInt16 a_RemotePort)
{
	// If not valid anymore, drop the connection:
	if (!m_Callbacks.IsValid())
	{
		return nullptr;
	}

	// Ask the plugin for link callbacks:
	cPluginLua::cOperation Op(m_Plugin);
	cLuaState::cRef LinkCallbacks;
	if (
		!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnIncomingConnection"), a_RemoteIPAddress, a_RemotePort, m_Port, cLuaState::Return, LinkCallbacks) ||
		!LinkCallbacks.IsValid()
	)
	{
		LOGINFO("cNetwork server (port %d) OnIncomingConnection callback failed in plugin %s. Dropping connection.",
			m_Port, m_Plugin.GetName().c_str()
		);
		return nullptr;
	}

	// Create the link wrapper to use with the callbacks:
	auto res = std::make_shared<cLuaTCPLink>(m_Plugin, std::move(LinkCallbacks), m_Self);

	// Add the link to the list of our connections:
	cCSLock Lock(m_CSConnections);
	m_Connections.push_back(res);

	return res;
}





void cLuaServerHandle::OnAccepted(cTCPLink & a_Link)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Notify the plugin:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnAccepted"), static_cast<cLuaTCPLink *>(a_Link.GetCallbacks().get())))
	{
		LOGINFO("cNetwork server (port %d) OnAccepted callback failed in plugin %s, connection to %s:%d.",
			m_Port, m_Plugin.GetName().c_str(), a_Link.GetRemoteIP().c_str(), a_Link.GetRemotePort()
		);
		return;
	}
}





void cLuaServerHandle::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Notify the plugin:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnError"), a_ErrorCode, a_ErrorMsg))
	{
		LOGINFO("cNetwork server (port %d) OnError callback failed in plugin %s. The error is %d (%s).",
			m_Port, m_Plugin.GetName().c_str(), a_ErrorCode, a_ErrorMsg.c_str()
		);
		return;
	}
}





