
// LuaTCPLink.cpp

// Implements the cLuaTCPLink class representing a Lua wrapper for the cTCPLink class and the callbacks it needs

#include "Globals.h"
#include "LuaTCPLink.h"
#include "LuaServerHandle.h"





cLuaTCPLink::cLuaTCPLink(cPluginLua & a_Plugin, int a_CallbacksTableStackPos):
	m_Plugin(a_Plugin),
	m_Callbacks(a_Plugin.GetLuaState(), a_CallbacksTableStackPos)
{
	// Warn if the callbacks aren't valid:
	if (!m_Callbacks.IsValid())
	{
		LOGWARNING("cTCPLink in plugin %s: callbacks could not be retrieved", m_Plugin.GetName().c_str());
		cPluginLua::cOperation Op(m_Plugin);
		Op().LogStackTrace();
	}
}





cLuaTCPLink::cLuaTCPLink(cPluginLua & a_Plugin, cLuaState::cRef && a_CallbacksTableRef, cLuaServerHandleWPtr a_ServerHandle):
	m_Plugin(a_Plugin),
	m_Callbacks(std::move(a_CallbacksTableRef)),
	m_Server(std::move(a_ServerHandle))
{
	// Warn if the callbacks aren't valid:
	if (!m_Callbacks.IsValid())
	{
		LOGWARNING("cTCPLink in plugin %s: callbacks could not be retrieved", m_Plugin.GetName().c_str());
		cPluginLua::cOperation Op(m_Plugin);
		Op().LogStackTrace();
	}
}





cLuaTCPLink::~cLuaTCPLink()
{
	// If the link is still open, close it:
	cTCPLinkPtr Link = m_Link;
	if (Link != nullptr)
	{
		Link->Close();
	}

	Terminated();
}





bool cLuaTCPLink::Send(const AString & a_Data)
{
	// Safely grab a copy of the link:
	cTCPLinkPtr Link = m_Link;
	if (Link == nullptr)
	{
		return false;
	}

	// Send the data:
	return Link->Send(a_Data);
}





AString cLuaTCPLink::GetLocalIP(void) const
{
	// Safely grab a copy of the link:
	cTCPLinkPtr Link = m_Link;
	if (Link == nullptr)
	{
		return "";
	}

	// Get the IP address:
	return Link->GetLocalIP();
}





UInt16 cLuaTCPLink::GetLocalPort(void) const
{
	// Safely grab a copy of the link:
	cTCPLinkPtr Link = m_Link;
	if (Link == nullptr)
	{
		return 0;
	}

	// Get the port:
	return Link->GetLocalPort();
}





AString cLuaTCPLink::GetRemoteIP(void) const
{
	// Safely grab a copy of the link:
	cTCPLinkPtr Link = m_Link;
	if (Link == nullptr)
	{
		return "";
	}

	// Get the IP address:
	return Link->GetRemoteIP();
}





UInt16 cLuaTCPLink::GetRemotePort(void) const
{
	// Safely grab a copy of the link:
	cTCPLinkPtr Link = m_Link;
	if (Link == nullptr)
	{
		return 0;
	}

	// Get the port:
	return Link->GetRemotePort();
}





void cLuaTCPLink::Shutdown(void)
{
	// Safely grab a copy of the link and shut it down:
	cTCPLinkPtr Link = m_Link;
	if (Link != nullptr)
	{
		Link->Shutdown();
	}

	Terminated();
}





void cLuaTCPLink::Close(void)
{
	// If the link is still open, close it:
	cTCPLinkPtr Link = m_Link;
	if (Link != nullptr)
	{
		Link->Close();
	}

	Terminated();
}





void cLuaTCPLink::Terminated(void)
{
	// Disable the callbacks:
	if (m_Callbacks.IsValid())
	{
		m_Callbacks.UnRef();
	}

	// If the managing server is still alive, let it know we're terminating:
	auto Server = m_Server.lock();
	if (Server != nullptr)
	{
		Server->RemoveLink(this);
	}

	// If the link is still open, close it:
	cTCPLinkPtr Link = m_Link;
	if (Link != nullptr)
	{
		Link->Close();
		m_Link.reset();
	}
}





void cLuaTCPLink::OnConnected(cTCPLink & a_Link)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnConnected"), this))
	{
		LOGINFO("cTCPLink OnConnected() callback failed in plugin %s.", m_Plugin.GetName().c_str());
	}
}





void cLuaTCPLink::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnError"), this, a_ErrorCode, a_ErrorMsg))
	{
		LOGINFO("cTCPLink OnError() callback failed in plugin %s; the link error is %d (%s).",
			m_Plugin.GetName().c_str(), a_ErrorCode, a_ErrorMsg.c_str()
		);
	}

	Terminated();
}





void cLuaTCPLink::OnLinkCreated(cTCPLinkPtr a_Link)
{
	// Store the cTCPLink for later use:
	m_Link = a_Link;
}





void cLuaTCPLink::OnReceivedData(const char * a_Data, size_t a_Length)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnReceivedData"), this, AString(a_Data, a_Length)))
	{
		LOGINFO("cTCPLink OnReceivedData callback failed in plugin %s.", m_Plugin.GetName().c_str());
	}
}





void cLuaTCPLink::OnRemoteClosed(void)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnRemoteClosed"), this))
	{
		LOGINFO("cTCPLink OnRemoteClosed() callback failed in plugin %s.", m_Plugin.GetName().c_str());
	}

	Terminated();
}




