
// UDPEndpointImpl.h

// Declares the cUDPEndpointImpl class representing an implementation of an endpoint in UDP communication





#pragma once

#include "Network.h"
#include <event2/event.h>





// fwd:
class cUDPEndpointImpl;
typedef std::shared_ptr<cUDPEndpointImpl> cUDPEndpointImplPtr;





class cUDPEndpointImpl:
	public cUDPEndpoint
{
	using Super = cUDPEndpoint;

public:

	/** Creates a new instance of the endpoint, with the specified callbacks.
	Tries to open on the specified port; if it fails, the endpoint is left in the "closed" state.
	If a_Port is 0, the OS is free to assign any port number it likes to the endpoint. */
	cUDPEndpointImpl(UInt16 a_Port, cUDPEndpoint::cCallbacks & a_Callbacks);

	virtual ~cUDPEndpointImpl() override;

	// cUDPEndpoint overrides:
	virtual void Close(void) override;
	virtual bool IsOpen(void) const override;
	virtual UInt16 GetPort(void) const override;
	virtual bool Send(const AString & a_Payload, const AString & a_Host, UInt16 a_Port) override;
	virtual void EnableBroadcasts(void) override;

protected:
	/** The local port on which the endpoint is open.
	If this is zero, it means the endpoint is closed - either opening has failed, or it has been closed explicitly. */
	UInt16 m_Port;

	/** The primary underlying OS socket. */
	evutil_socket_t m_MainSock;

	/** True if m_MainSock is in the IPv6 namespace (needs IPv6 addresses for sending). */
	bool m_IsMainSockIPv6;

	/** The secondary OS socket (if primary doesn't support dualstack). */
	evutil_socket_t m_SecondarySock;

	/** The LibEvent handle for the primary socket. */
	event * m_MainEvent;

	/** The LibEvent handle for the secondary socket. */
	event * m_SecondaryEvent;


	/** Creates and opens the socket on the specified port.
	If a_Port is 0, the OS is free to assign any port number it likes to the endpoint.
	If the opening fails, the OnError() callback is called and the endpoint is left "closed" (IsOpen() returns false). */
	void Open(UInt16 a_Port);

	/** The callback that LibEvent calls when an event occurs on one of the sockets.
	Calls Callback() on a_Self. */
	static void RawCallback(evutil_socket_t a_Socket, short a_What, void * a_Self);

	/** The callback that is called when an event occurs on one of the sockets. */
	void Callback(evutil_socket_t a_Socket, short a_What);
};




