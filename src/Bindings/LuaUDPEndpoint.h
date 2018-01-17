
// LuaUDPEndpoint.h

// Declares the cLuaUDPEndpoint class representing a Lua wrapper for the cUDPEndpoint class and the callbacks it needs





#pragma once

#include "../OSSupport/Network.h"
#include "LuaState.h"





// fwd:
class cLuaUDPEndpoint;
typedef std::shared_ptr<cLuaUDPEndpoint> cLuaUDPEndpointPtr;





class cLuaUDPEndpoint:
	public cUDPEndpoint::cCallbacks
{
public:
	/** Creates a new instance of the endpoint, wrapping the callbacks that are in the specified table. */
	cLuaUDPEndpoint(cLuaState::cTableRefPtr && a_Callbacks);

	virtual ~cLuaUDPEndpoint() override;

	/** Opens the endpoint so that it starts listening for incoming data on the specified port.
	a_Self is the shared pointer to self that the object keeps to keep itself alive for as long as it needs (for Lua). */
	bool Open(UInt16 a_Port, cLuaUDPEndpointPtr a_Self);

	/** Sends the data contained in the string to the specified remote peer.
	Returns true if successful, false on immediate failure (queueing the data failed etc.) */
	bool Send(const AString & a_Data, const AString & a_RemotePeer, UInt16 a_RemotePort);

	/** Returns the port on which endpoint is listening for incoming data. */
	UInt16 GetPort(void) const;

	/** Returns true if the endpoint is open for incoming data. */
	bool IsOpen(void) const;

	/** Closes the UDP listener. */
	void Close(void);

	/** Enables outgoing broadcasts to be made using this endpoint. */
	void EnableBroadcasts(void);

	/** Called when Lua garbage-collects the object.
	Releases the internal SharedPtr to self, so that the instance may be deallocated. */
	void Release(void);

protected:
	/** The Lua table that holds the callbacks to be invoked. */
	cLuaState::cTableRefPtr m_Callbacks;

	/** SharedPtr to self, so that the object can keep itself alive for as long as it needs (for Lua). */
	cLuaUDPEndpointPtr m_Self;

	/** The underlying network endpoint.
	May be nullptr. */
	cUDPEndpointPtr m_Endpoint;


	/** Common code called when the endpoint is considered as terminated.
	Releases m_Endpoint and m_Callbacks, each when applicable. */
	void Terminated(void);

	// cUDPEndpoint::cCallbacks overrides:
	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override;
	virtual void OnReceivedData(const char * a_Data, size_t a_Size, const AString & a_RemotePeer, UInt16 a_RemotePort) override;
};




