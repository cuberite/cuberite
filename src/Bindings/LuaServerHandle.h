
// LuaServerHandle.h

// Declares the cLuaServerHandle class wrapping the cServerHandle functionality for Lua API





#pragma once

#include "../OSSupport/Network.h"
#include "LuaState.h"





// fwd:
class cLuaTCPLink;
typedef std::shared_ptr<cLuaTCPLink> cLuaTCPLinkPtr;
typedef std::vector<cLuaTCPLinkPtr> cLuaTCPLinkPtrs;
class cLuaServerHandle;
typedef std::shared_ptr<cLuaServerHandle> cLuaServerHandlePtr;




class cLuaServerHandle:
	public cNetwork::cListenCallbacks
{
public:
	/** Creates a new instance of the server handle,
	wrapping the (listen-) callbacks that are in the specified table. */
	cLuaServerHandle(UInt16 a_Port, cLuaState::cTableRefPtr && a_Callbacks);

	virtual ~cLuaServerHandle() override;

	/** Called by cNetwork::Listen()'s binding.
	Sets the server handle around which this instance is wrapped, and a self SharedPtr for link management. */
	void SetServerHandle(cServerHandlePtr a_ServerHandle, cLuaServerHandlePtr a_Self);

	/** Terminates all connections and closes the listening socket. */
	void Close(void);

	/** Returns true if the server is currently listening. */
	bool IsListening(void);

	/** Removes the link from the list of links that the server is currently tracking. */
	void RemoveLink(cLuaTCPLink * a_Link);

	/** Called when Lua garbage-collects the object.
	Releases the internal SharedPtr to self, so that the instance may be deallocated. */
	void Release(void);

protected:
	/** The Lua table that holds the callbacks to be invoked. */
	cLuaState::cTableRefPtr m_Callbacks;

	/** The port on which the server is listening.
	Used mainly for better error reporting. */
	UInt16 m_Port;

	/** The cServerHandle around which this instance is wrapped. */
	cServerHandlePtr m_ServerHandle;

	/** Protects m_Connections against multithreaded access. */
	cCriticalSection m_CSConnections;

	/** All connections that are currently active in this server.
	Protected by m_CSConnections. */
	cLuaTCPLinkPtrs m_Connections;

	/** SharedPtr to self, given out to newly created links. */
	cLuaServerHandlePtr m_Self;


	// cNetwork::cListenCallbacks overrides:
	virtual cTCPLink::cCallbacksPtr OnIncomingConnection(const AString & a_RemoteIPAddress, UInt16 a_RemotePort) override;
	virtual void OnAccepted(cTCPLink & a_Link) override;
	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override;
};




