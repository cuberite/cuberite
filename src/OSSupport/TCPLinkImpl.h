
// TCPLinkImpl.h

// Declares the cTCPLinkImpl class implementing the TCP link functionality

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead





#pragma once

#include "Network.h"
#include <event2/event.h>
#include <event2/bufferevent.h>





// fwd:
class cServerHandleImpl;
typedef SharedPtr<cServerHandleImpl> cServerHandleImplPtr;
class cTCPLinkImpl;
typedef SharedPtr<cTCPLinkImpl> cTCPLinkImplPtr;
typedef std::vector<cTCPLinkImplPtr> cTCPLinkImplPtrs;





class cTCPLinkImpl:
	public cTCPLink
{
	typedef cTCPLink super;

public:
	/** Creates a new link based on the given socket.
	Used for connections accepted in a server using cNetwork::Listen().
	a_Address and a_AddrLen describe the remote peer that has connected.
	The link is created disabled, you need to call Enable() to start the regular communication. */
	cTCPLinkImpl(evutil_socket_t a_Socket, cCallbacksPtr a_LinkCallbacks, cServerHandleImplPtr a_Server, const sockaddr * a_Address, socklen_t a_AddrLen);

	/** Destroys the LibEvent handle representing the link. */
	~cTCPLinkImpl();

	/** Queues a connection request to the specified host.
	a_ConnectCallbacks must be valid.
	Returns a link that has the connection request queued, or NULL for failure. */
	static cTCPLinkImplPtr Connect(const AString & a_Host, UInt16 a_Port, cTCPLink::cCallbacksPtr a_LinkCallbacks, cNetwork::cConnectCallbacksPtr a_ConnectCallbacks);

	/** Enables communication over the link.
	Links are created with communication disabled, so that creation callbacks can be called first.
	This function then enables the regular communication to be reported.
	The a_Self parameter is used so that the socket can keep itself alive as long as the callbacks are coming. */
	void Enable(cTCPLinkImplPtr a_Self);

	// cTCPLink overrides:
	virtual bool Send(const void * a_Data, size_t a_Length) override;
	virtual AString GetLocalIP(void) const override { return m_LocalIP; }
	virtual UInt16 GetLocalPort(void) const override { return m_LocalPort; }
	virtual AString GetRemoteIP(void) const override { return m_RemoteIP; }
	virtual UInt16 GetRemotePort(void) const override { return m_RemotePort; }
	virtual void Shutdown(void) override;
	virtual void Close(void) override;

protected:

	/** Callbacks to call when the connection is established.
	May be NULL if not used. Only used for outgoing connections (cNetwork::Connect()). */
	cNetwork::cConnectCallbacksPtr m_ConnectCallbacks;

	/** The LibEvent handle representing this connection. */
	bufferevent * m_BufferEvent;

	/** The server handle that has created this link.
	Only valid for incoming connections, nullptr for outgoing connections. */
	cServerHandleImplPtr m_Server;

	/** The IP address of the local endpoint. Valid only after the socket has been connected. */
	AString m_LocalIP;

	/** The port of the local endpoint. Valid only after the socket has been connected. */
	UInt16 m_LocalPort;

	/** The IP address of the remote endpoint. Valid only after the socket has been connected. */
	AString m_RemoteIP;

	/** The port of the remote endpoint. Valid only after the socket has been connected. */
	UInt16 m_RemotePort;

	/** SharedPtr to self, used to keep this object alive as long as the callbacks are coming.
	Initialized in Enable(), cleared in Close() and EventCallback(RemoteClosed). */
	cTCPLinkImplPtr m_Self;

	/** If true, Shutdown() has been called and is in queue.
	No more data is allowed to be sent via Send() and after all the currently buffered
	data is sent to the OS TCP stack, the socket gets shut down. */
	bool m_ShouldShutdown;


	/** Creates a new link to be queued to connect to a specified host:port.
	Used for outgoing connections created using cNetwork::Connect().
	To be used only by the Connect() factory function.
	The link is created disabled, you need to call Enable() to start the regular communication. */
	cTCPLinkImpl(const cCallbacksPtr a_LinkCallbacks);

	/** Callback that LibEvent calls when there's data available from the remote peer. */
	static void ReadCallback(bufferevent * a_BufferEvent, void * a_Self);

	/** Callback that LibEvent calls when the remote peer can receive more data. */
	static void WriteCallback(bufferevent * a_BufferEvent, void * a_Self);

	/** Callback that LibEvent calls when there's a non-data-related event on the socket. */
	static void EventCallback(bufferevent * a_BufferEvent, short a_What, void * a_Self);

	/** Sets a_IP and a_Port to values read from a_Address, based on the correct address family. */
	static void UpdateAddress(const sockaddr * a_Address, socklen_t a_AddrLen, AString & a_IP, UInt16 & a_Port);

	/** Updates m_LocalIP and m_LocalPort based on the metadata read from the socket. */
	void UpdateLocalAddress(void);

	/** Updates m_RemoteIP and m_RemotePort based on the metadata read from the socket. */
	void UpdateRemoteAddress(void);

	/** Calls shutdown on the link and disables LibEvent writing.
	Called after all data from LibEvent buffers is sent to the OS TCP stack and shutdown() has been called before. */
	void DoActualShutdown(void);
};




