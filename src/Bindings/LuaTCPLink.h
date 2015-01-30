
// LuaTCPLink.h

// Declares the cLuaTCPLink class representing a Lua wrapper for the cTCPLink class and the callbacks it needs





#pragma once

#include "../OSSupport/Network.h"
#include "PluginLua.h"





// fwd:
class cLuaServerHandle;
typedef WeakPtr<cLuaServerHandle> cLuaServerHandleWPtr;





class cLuaTCPLink:
	public cNetwork::cConnectCallbacks,
	public cTCPLink::cCallbacks
{
public:
	/** Creates a new instance of the link, attached to the specified plugin and wrapping the callbacks that are in a table at the specified stack pos. */
	cLuaTCPLink(cPluginLua & a_Plugin, int a_CallbacksTableStackPos);

	/** Creates a new instance of the link, attached to the specified plugin and wrapping the callbacks that are in the specified referenced table. */
	cLuaTCPLink(cPluginLua & a_Plugin, cLuaState::cRef && a_CallbacksTableRef, cLuaServerHandleWPtr a_Server);

	~cLuaTCPLink();

	/** Sends the data contained in the string to the remote peer.
	Returns true if successful, false on immediate failure (queueing the data failed or link not available). */
	bool Send(const AString & a_Data);

	/** Returns the IP address of the local endpoint of the connection. */
	AString GetLocalIP(void) const;

	/** Returns the port used by the local endpoint of the connection. */
	UInt16 GetLocalPort(void) const;

	/** Returns the IP address of the remote endpoint of the connection. */
	AString GetRemoteIP(void) const;

	/** Returns the port used by the remote endpoint of the connection. */
	UInt16 GetRemotePort(void) const;

	/** Closes the link gracefully.
	The link will send any queued outgoing data, then it will send the FIN packet.
	The link will still receive incoming data from remote until the remote closes the connection. */
	void Shutdown(void);

	/** Drops the connection without any more processing.
	Sends the RST packet, queued outgoing and incoming data is lost. */
	void Close(void);

protected:
	/** The plugin for which the link is created. */
	cPluginLua & m_Plugin;

	/** The Lua table that holds the callbacks to be invoked. */
	cLuaState::cRef m_Callbacks;

	/** The underlying link representing the connection.
	May be nullptr. */
	cTCPLinkPtr m_Link;

	/** The server that is responsible for this link, if any. */
	cLuaServerHandleWPtr m_Server;


	/** Common code called when the link is considered as terminated.
	Releases m_Link, m_Callbacks and this from m_Server, each when applicable. */
	void Terminated(void);

	// cNetwork::cConnectCallbacks overrides:
	virtual void OnConnected(cTCPLink & a_Link) override;
	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override;

	// cTCPLink::cCallbacks overrides:
	virtual void OnLinkCreated(cTCPLinkPtr a_Link) override;
	virtual void OnReceivedData(const char * a_Data, size_t a_Length) override;
	virtual void OnRemoteClosed(void) override;
	// The OnError() callback is shared with cNetwork::cConnectCallbacks
};




