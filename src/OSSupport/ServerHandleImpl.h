
// ServerHandleImpl.h

// Declares the cServerHandleImpl class implementing the TCP server functionality

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead





#pragma once

#include "Network.h"
#include <event2/listener.h>
#include "CriticalSection.h"





// fwd:
class cTCPLinkImpl;
typedef std::shared_ptr<cTCPLinkImpl> cTCPLinkImplPtr;
typedef std::vector<cTCPLinkImplPtr> cTCPLinkImplPtrs;
class cServerHandleImpl;
typedef std::shared_ptr<cServerHandleImpl> cServerHandleImplPtr;
typedef std::vector<cServerHandleImplPtr> cServerHandleImplPtrs;





class cServerHandleImpl:
	public cServerHandle
{
	using Super = cServerHandle;
	friend class cTCPLinkImpl;

public:

	/** Closes the server, dropping all the connections. */
	virtual ~cServerHandleImpl() override;

	/** Creates a new server instance listening on the specified port.
	Both IPv4 and IPv6 interfaces are used, if possible.
	Always returns a server instance; in the event of a failure, the instance holds the error details. Use IsListening() to query success. */
	static cServerHandleImplPtr Listen(
		UInt16 a_Port,
		cNetwork::cListenCallbacksPtr a_ListenCallbacks
	);

	// cServerHandle overrides:
	virtual void Close(void) override;
	virtual bool IsListening(void) const override { return m_IsListening; }

protected:
	/** The callbacks used to notify about incoming connections. */
	cNetwork::cListenCallbacksPtr m_ListenCallbacks;

	/** The LibEvent handle representing the main listening socket. */
	evconnlistener * m_ConnListener;

	/** The LibEvent handle representing the secondary listening socket (only when side-by-side listening is needed, such as WinXP). */
	evconnlistener * m_SecondaryConnListener;

	/** Set to true when the server is initialized successfully and is listening for incoming connections. */
	bool m_IsListening;

	/** Container for all currently active connections on this server. */
	cTCPLinkImplPtrs m_Connections;

	/** Mutex protecting m_Connections againt multithreaded access. */
	cCriticalSection m_CS;

	/** Contains the error code for the failure to listen. Only valid for non-listening instances. */
	int m_ErrorCode;

	/** Contains the error message for the failure to listen. Only valid for non-listening instances. */
	AString m_ErrorMsg;

	/** The SharedPtr to self, so that it can be passed to created links. */
	cServerHandleImplPtr m_SelfPtr;



	/** Creates a new instance with the specified callbacks.
	Initializes the internals, but doesn't start listening yet. */
	cServerHandleImpl(cNetwork::cListenCallbacksPtr a_ListenCallbacks);

	/** Starts listening on the specified port.
	Returns true if successful, false on failure. On failure, sets m_ErrorCode and m_ErrorMsg. */
	bool Listen(UInt16 a_Port);

	/** The callback called by LibEvent upon incoming connection. */
	static void Callback(evconnlistener * a_Listener, evutil_socket_t a_Socket, sockaddr * a_Addr, int a_Len, void * a_Self);

	/** Removes the specified link from m_Connections.
	Called by cTCPLinkImpl when the link is terminated. */
	void RemoveLink(const cTCPLinkImpl * a_Link);
};





