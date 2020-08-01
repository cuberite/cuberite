
// ManualBindings_Network.cpp

// Implements the cNetwork-related API bindings for Lua
// Also implements the cUrlClient bindings

#include "Globals.h"
#include "ManualBindings.h"
#include "tolua++/include/tolua++.h"
#include "LuaState.h"
#include "LuaTCPLink.h"
#include "LuaNameLookup.h"
#include "LuaServerHandle.h"
#include "LuaUDPEndpoint.h"
#include "../HTTP/UrlClient.h"





////////////////////////////////////////////////////////////////////////////////
// cNetwork API functions:

/** Binds cNetwork::Connect */
static int tolua_cNetwork_Connect(lua_State * L)
{
	// Function signature:
	// cNetwork:Connect(Host, Port, Callbacks) -> bool

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cNetwork") ||
		!S.CheckParamString(2) ||
		!S.CheckParamNumber(3) ||
		!S.CheckParamTable(4) ||
		!S.CheckParamEnd(5)
	)
	{
		return 0;
	}

	// Read the params:
	AString host;
	int port = 0;
	cLuaState::cTableRefPtr callbacks;
	if (!S.GetStackValues(2, host, port, callbacks))
	{
		return S.ApiParamError("Cannot read parameters.");
	}

	// Check validity:
	if ((port < 0) || (port > 65535))
	{
		return S.ApiParamError("Port number out of range (%d, range 0 - 65535)", port);
	}
	ASSERT(callbacks != nullptr);  // Invalid callbacks would have resulted in GetStackValues() returning false

	// Create the LuaTCPLink glue class:
	auto link = std::make_shared<cLuaTCPLink>(std::move(callbacks));

	// Try to connect:
	bool res = cNetwork::Connect(host, static_cast<UInt16>(port), link, link);
	S.Push(res);

	return 1;
}





/** Binds cNetwork::CreateUDPEndpoint */
static int tolua_cNetwork_CreateUDPEndpoint(lua_State * L)
{
	// Function signature:
	// cNetwork:CreateUDPEndpoint(Port, Callbacks) -> cUDPEndpoint

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cNetwork") ||
		!S.CheckParamNumber(2) ||
		!S.CheckParamTable(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Read the params:
	int port;
	cLuaState::cTableRefPtr callbacks;
	if (!S.GetStackValues(2, port, callbacks))
	{
		return S.ApiParamError("Cannot read parameters");
	}

	// Check validity:
	if ((port < 0) || (port > 65535))
	{
		return S.ApiParamError("Port number out of range (%d, range 0 - 65535)", port);
	}
	ASSERT(callbacks != nullptr);  // Invalid callbacks would have resulted in GetStackValues() returning false

	// Create the LuaUDPEndpoint glue class:
	auto endpoint = std::make_shared<cLuaUDPEndpoint>(std::move(callbacks));
	endpoint->Open(static_cast<UInt16>(port), endpoint);

	// Register the endpoint to be garbage-collected by Lua:
	tolua_pushusertype(L, endpoint.get(), "cUDPEndpoint");
	tolua_register_gc(L, lua_gettop(L));

	// Return the endpoint object:
	S.Push(endpoint.get());
	return 1;
}





/** Binds cNetwork::EnumLocalIPAddresses */
static int tolua_cNetwork_EnumLocalIPAddresses(lua_State * L)
{
	// Function signature:
	// cNetwork:EnumLocalIPAddresses() -> {string, ...}

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cNetwork") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Push the enumerated addresses:
	S.Push(cNetwork::EnumLocalIPAddresses());
	return 1;
}





/** Binds cNetwork::HostnameToIP */
static int tolua_cNetwork_HostnameToIP(lua_State * L)
{
	// Function signature:
	// cNetwork:HostnameToIP(Host, Callbacks) -> bool

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cNetwork") ||
		!S.CheckParamString(2) ||
		!S.CheckParamTable(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Read the params:
	AString host;
	cLuaState::cTableRefPtr callbacks;
	if (!S.GetStackValues(2, host, callbacks))
	{
		return S.ApiParamError("Cannot read parameters.");
	}
	ASSERT(callbacks != nullptr);  // Invalid callbacks would have resulted in GetStackValues() returning false

	// Try to look up:
	bool res = cNetwork::HostnameToIP(host, std::make_shared<cLuaNameLookup>(host, std::move(callbacks)));
	S.Push(res);
	return 1;
}





/** Binds cNetwork::IPToHostname */
static int tolua_cNetwork_IPToHostname(lua_State * L)
{
	// Function signature:
	// cNetwork:IPToHostname(IP, Callbacks) -> bool

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cNetwork") ||
		!S.CheckParamString(2) ||
		!S.CheckParamTable(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Read the params:
	AString ip;
	cLuaState::cTableRefPtr callbacks;
	if (!S.GetStackValues(2, ip, callbacks))
	{
		return S.ApiParamError("Cannot read parameters.");
	}
	ASSERT(callbacks != nullptr);  // Invalid callbacks would have resulted in GetStackValues() returning false

	// Try to look up:
	bool res = cNetwork::IPToHostName(ip, std::make_shared<cLuaNameLookup>(ip, std::move(callbacks)));
	S.Push(res);
	return 1;
}





/** Binds cNetwork::Listen */
static int tolua_cNetwork_Listen(lua_State * L)
{
	// Function signature:
	// cNetwork:Listen(Port, Callbacks) -> cServerHandle

	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cNetwork") ||
		!S.CheckParamNumber(2) ||
		!S.CheckParamTable(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Read the params:
	int port = 0;
	cLuaState::cTableRefPtr callbacks;
	if (!S.GetStackValues(2, port, callbacks))
	{
		return S.ApiParamError("Cannot read parameters");
	}

	// Check the validity:
	if ((port < 0) || (port > 65535))
	{
		return S.ApiParamError("Port number out of range (%d, range 0 - 65535)", port);
	}
	auto port16 = static_cast<UInt16>(port);

	// Create the LuaTCPLink glue class:
	auto srv = std::make_shared<cLuaServerHandle>(port16, std::move(callbacks));

	// Listen:
	srv->SetServerHandle(cNetwork::Listen(port16, srv), srv);

	// Register the server to be garbage-collected by Lua:
	tolua_pushusertype(L, srv.get(), "cServerHandle");
	tolua_register_gc(L, lua_gettop(L));

	// Return the server handle wrapper:
	S.Push(srv.get());
	return 1;
}





////////////////////////////////////////////////////////////////////////////////
// cServerHandle bindings (routed through cLuaServerHandle):

/** Called when Lua destroys the object instance.
Close the server and let it deallocate on its own (it's in a SharedPtr). */
static int tolua_collect_cServerHandle(lua_State * L)
{
	auto Srv = static_cast<cLuaServerHandle *>(tolua_tousertype(L, 1, nullptr));
	ASSERT(Srv != nullptr);
	Srv->Release();
	return 0;
}





/** Binds cLuaServerHandle::Close */
static int tolua_cServerHandle_Close(lua_State * L)
{
	// Function signature:
	// ServerInstance:Close()

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cServerHandle") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the server handle:
	auto Srv = *static_cast<cLuaServerHandle **>(lua_touserdata(L, 1));
	ASSERT(Srv != nullptr);  // Checked by CheckParamSelf()

	// Close it:
	Srv->Close();
	return 0;
}





/** Binds cLuaServerHandle::IsListening */
static int tolua_cServerHandle_IsListening(lua_State * L)
{
	// Function signature:
	// ServerInstance:IsListening() -> bool

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cServerHandle") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the server handle:
	auto Srv = *static_cast<cLuaServerHandle **>(lua_touserdata(L, 1));
	ASSERT(Srv != nullptr);  // Checked by CheckParamSelf()

	// Query it:
	S.Push(Srv->IsListening());
	return 1;
}





////////////////////////////////////////////////////////////////////////////////
// cTCPLink bindings (routed through cLuaTCPLink):

/** Binds cLuaTCPLink::Close */
static int tolua_cTCPLink_Close(lua_State * L)
{
	// Function signature:
	// LinkInstance:Close()

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the link:
	auto Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));
	ASSERT(Link != nullptr);  // Checked by CheckParamSelf()

	// Close the link:
	Link->Close();
	return 0;
}





/** Binds cLuaTCPLink::GetLocalIP */
static int tolua_cTCPLink_GetLocalIP(lua_State * L)
{
	// Function signature:
	// LinkInstance:GetLocalIP() -> string

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the link:
	auto Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));
	ASSERT(Link != nullptr);  // Checked by CheckParamSelf()

	// Get the IP:
	S.Push(Link->GetLocalIP());
	return 1;
}





/** Binds cLuaTCPLink::GetLocalPort */
static int tolua_cTCPLink_GetLocalPort(lua_State * L)
{
	// Function signature:
	// LinkInstance:GetLocalPort() -> number

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the link:
	auto Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));
	ASSERT(Link != nullptr);  // Checked by CheckParamSelf()

	// Get the Port:
	S.Push(Link->GetLocalPort());
	return 1;
}





/** Binds cLuaTCPLink::GetRemoteIP */
static int tolua_cTCPLink_GetRemoteIP(lua_State * L)
{
	// Function signature:
	// LinkInstance:GetRemoteIP() -> string

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the link:
	auto Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));
	ASSERT(Link != nullptr);  // Checked by CheckParamSelf()

	// Get the IP:
	S.Push(Link->GetRemoteIP());
	return 1;
}





/** Binds cLuaTCPLink::GetRemotePort */
static int tolua_cTCPLink_GetRemotePort(lua_State * L)
{
	// Function signature:
	// LinkInstance:GetRemotePort() -> number

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the link:
	auto Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));
	ASSERT(Link != nullptr);  // Checked by CheckParamSelf()

	// Get the Port:
	S.Push(Link->GetRemotePort());
	return 1;
}





/** Binds cLuaTCPLink::Send */
static int tolua_cTCPLink_Send(lua_State * L)
{
	// Function signature:
	// LinkInstance:Send(DataString)

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cTCPLink") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the link:
	auto Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));
	ASSERT(Link != nullptr);  // Checked by CheckParamSelf()

	// Get the data to send:
	AString Data;
	S.GetStackValues(2, Data);

	// Send the data:
	Link->Send(Data);
	return 0;
}





/** Binds cLuaTCPLink::Shutdown */
static int tolua_cTCPLink_Shutdown(lua_State * L)
{
	// Function signature:
	// LinkInstance:Shutdown()

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the link:
	auto Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));
	ASSERT(Link != nullptr);  // Checked by CheckParamSelf()

	// Shutdown the link:
	Link->Shutdown();
	return 0;
}





/** Binds cLuaTCPLink::StartTLSClient */
static int tolua_cTCPLink_StartTLSClient(lua_State * L)
{
	// Function signature:
	// LinkInstance:StartTLSClient(OwnCert, OwnPrivKey, OwnPrivKeyPassword) -> [true] or [nil, ErrMsg]

	// Get the link:
	cLuaState S(L);
	if (!S.CheckParamSelf("cTCPLink"))
	{
		return 0;
	}
	auto Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));
	ASSERT(Link != nullptr);  // Checked by CheckParamSelf()

	// Read the (optional) params:
	AString OwnCert, OwnPrivKey, OwnPrivKeyPassword;
	S.GetStackValues(2, OwnCert, OwnPrivKey, OwnPrivKeyPassword);

	// Start the TLS handshake:
	AString res = Link->StartTLSClient(OwnCert, OwnPrivKey, OwnPrivKeyPassword);
	if (!res.empty())
	{
		S.Push(cLuaState::Nil, Printf("Cannot start TLS on link to %s:%d: %s", Link->GetRemoteIP().c_str(), Link->GetRemotePort(), res.c_str()));
		return 2;
	}
	return 1;
}





/** Binds cLuaTCPLink::StartTLSServer */
static int tolua_cTCPLink_StartTLSServer(lua_State * L)
{
	// Function signature:
	// LinkInstance:StartTLSServer(OwnCert, OwnPrivKey, OwnPrivKeyPassword, StartTLSData) -> [true] or [nil, ErrMsg]

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cTCPLink") ||
		!S.CheckParamString(2, 4) ||
		// Param 5 is optional, don't check
		!S.CheckParamEnd(6)
	)
	{
		return 0;
	}

	// Get the link:
	auto Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));
	ASSERT(Link != nullptr);  // Checked by CheckParamSelf()

	// Read the params:
	AString OwnCert, OwnPrivKey, OwnPrivKeyPassword, StartTLSData;
	S.GetStackValues(2, OwnCert, OwnPrivKey, OwnPrivKeyPassword, StartTLSData);

	// Start the TLS handshake:
	AString res = Link->StartTLSServer(OwnCert, OwnPrivKey, OwnPrivKeyPassword, StartTLSData);
	if (!res.empty())
	{
		S.Push(cLuaState::Nil, Printf("Cannot start TLS on link to %s:%d: %s", Link->GetRemoteIP().c_str(), Link->GetRemotePort(), res.c_str()));
		return 2;
	}
	S.Push(true);
	return 1;
}





////////////////////////////////////////////////////////////////////////////////
// cUDPEndpoint bindings (routed through cLuaUDPEndpoint):

/** Called when Lua destroys the object instance.
Close the endpoint and let it deallocate on its own (it's in a SharedPtr). */
static int tolua_collect_cUDPEndpoint(lua_State * L)
{
	auto endpoint = static_cast<cLuaUDPEndpoint *>(tolua_tousertype(L, 1, nullptr));
	ASSERT(endpoint != nullptr);
	endpoint->Release();
	return 0;
}





/** Binds cLuaUDPEndpoint::Close */
static int tolua_cUDPEndpoint_Close(lua_State * L)
{
	// Function signature:
	// EndpointInstance:Close()

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cUDPEndpoint") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the endpoint:
	auto endpoint = *static_cast<cLuaUDPEndpoint **>(lua_touserdata(L, 1));
	ASSERT(endpoint != nullptr);

	// Close it:
	endpoint->Close();
	return 0;
}





/** Binds cLuaUDPEndpoint::EnableBroadcasts */
static int tolua_cUDPEndpoint_EnableBroadcasts(lua_State * L)
{
	// Function signature:
	// EndpointInstance:EnableBroadcasts()

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cUDPEndpoint") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the endpoint:
	auto endpoint = *static_cast<cLuaUDPEndpoint **>(lua_touserdata(L, 1));
	ASSERT(endpoint != nullptr);

	// Enable the broadcasts:
	endpoint->EnableBroadcasts();
	return 0;
}





/** Binds cLuaUDPEndpoint::GetPort */
static int tolua_cUDPEndpoint_GetPort(lua_State * L)
{
	// Function signature:
	// Endpoint:GetPort() -> number

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cUDPEndpoint") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the endpoint:
	auto endpoint = *static_cast<cLuaUDPEndpoint **>(lua_touserdata(L, 1));
	ASSERT(endpoint != nullptr);

	// Get the Port:
	S.Push(endpoint->GetPort());
	return 1;
}





/** Binds cLuaUDPEndpoint::IsOpen */
static int tolua_cUDPEndpoint_IsOpen(lua_State * L)
{
	// Function signature:
	// Endpoint:IsOpen() -> bool

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cUDPEndpoint") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the endpoint:
	auto endpoint = *static_cast<cLuaUDPEndpoint **>(lua_touserdata(L, 1));
	ASSERT(endpoint != nullptr);

	// Close it:
	S.Push(endpoint->IsOpen());
	return 1;
}





/** Binds cLuaUDPEndpoint::Send */
static int tolua_cUDPEndpoint_Send(lua_State * L)
{
	// Function signature:
	// Endpoint:Send(DataString)

	cLuaState S(L);
	if (
		!S.CheckParamSelf("cUDPEndpoint") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamNumber(4) ||
		!S.CheckParamEnd(5)
	)
	{
		return 0;
	}

	// Get the link:
	auto endpoint = *static_cast<cLuaUDPEndpoint **>(lua_touserdata(L, 1));
	ASSERT(endpoint != nullptr);

	// Get the data to send:
	AString data, remotePeer;
	int remotePort = 0;
	S.GetStackValues(2, data, remotePeer, remotePort);

	// Check the port:
	if ((remotePort < 0) || (remotePort > USHRT_MAX))
	{
		return S.ApiParamError("Port number out of range (%d, range 0 - 65535)", remotePort);
	}

	// Send the data:
	S.Push(endpoint->Send(data, remotePeer, static_cast<UInt16>(remotePort)));
	return 1;
}





/** Used when the cUrlClient Lua request wants all the callbacks.
Maps each callback onto a Lua function callback in the callback table. */
class cFullUrlClientCallbacks:
	public cUrlClient::cCallbacks
{
public:
	/** Creates a new instance bound to the specified table of callbacks. */
	cFullUrlClientCallbacks(cLuaState::cTableRefPtr && a_Callbacks):
		m_Callbacks(std::move(a_Callbacks))
	{
	}


	// cUrlClient::cCallbacks overrides:
	virtual void OnConnected(cTCPLink & a_Link) override
	{
		// TODO: Cannot push a cTCPLink to Lua, need to translate via cLuaTCPLink
		m_Callbacks->CallTableFnWithSelf("OnConnected", cLuaState::Nil, a_Link.GetRemoteIP(), a_Link.GetRemotePort());
	}


	virtual bool OnCertificateReceived() override
	{
		// TODO: The received cert needs proper type specification from the underlying cUrlClient framework and in the Lua engine as well
		bool res = true;
		m_Callbacks->CallTableFnWithSelf("OnCertificateReceived", cLuaState::Return, res);
		return res;
	}


	virtual void OnTlsHandshakeCompleted() override
	{
		m_Callbacks->CallTableFnWithSelf("OnTlsHandshakeCompleted");
	}


	virtual void OnRequestSent() override
	{
		m_Callbacks->CallTableFnWithSelf("OnRequestSent");
	}


	virtual void OnStatusLine(const AString & a_HttpVersion, int a_StatusCode, const AString & a_Rest) override
	{
		m_Callbacks->CallTableFnWithSelf("OnStatusLine", a_HttpVersion, a_StatusCode, a_Rest);
	}


	virtual void OnHeader(const AString & a_Key, const AString & a_Value) override
	{
		m_Callbacks->CallTableFnWithSelf("OnHeader", a_Key, a_Value);
		m_Headers[a_Key] = a_Value;
	}


	virtual void OnHeadersFinished() override
	{
		m_Callbacks->CallTableFnWithSelf("OnHeadersFinished", m_Headers);
	}


	virtual void OnBodyData(const void * a_Data, size_t a_Size) override
	{
		m_Callbacks->CallTableFnWithSelf("OnBodyData", AString(static_cast<const char *>(a_Data), a_Size));
	}


	virtual void OnBodyFinished() override
	{
		m_Callbacks->CallTableFnWithSelf("OnBodyFinished");
	}


	virtual void OnError(const AString & a_ErrorMsg) override
	{
		m_Callbacks->CallTableFnWithSelf("OnError", a_ErrorMsg);
	}


	virtual void OnRedirecting(const AString & a_NewLocation) override
	{
		m_Callbacks->CallTableFnWithSelf("OnRedirecting", a_NewLocation);
	}


protected:
	/** The Lua table containing the callbacks. */
	cLuaState::cTableRefPtr m_Callbacks;

	/** Accumulator for all the headers to be reported in the OnHeadersFinished() callback. */
	AStringMap m_Headers;
};





/** Used when the cUrlClient Lua request has just a single callback.
The callback is used to report the entire body at once, together with the HTTP headers, or to report an error:
callback("BodyContents", {headers})
callback(nil, "ErrorMessage")
Accumulates the body contents into a single string until the body is finished.
Accumulates all HTTP headers into an AStringMap. */
class cSimpleUrlClientCallbacks:
	public cUrlClient::cCallbacks
{
public:
	/** Creates a new instance that uses the specified callback to report when request finishes. */
	cSimpleUrlClientCallbacks(cLuaState::cCallbackPtr && a_Callback):
		m_Callback(std::move(a_Callback))
	{
	}


	virtual void OnHeader(const AString & a_Key, const AString & a_Value) override
	{
		m_Headers[a_Key] = a_Value;
	}


	virtual void OnBodyData(const void * a_Data, size_t a_Size) override
	{
		m_ResponseBody.append(static_cast<const char *>(a_Data), a_Size);
	}


	virtual void OnBodyFinished() override
	{
		m_Callback->Call(m_ResponseBody, m_Headers);
	}


	virtual void OnError(const AString & a_ErrorMsg) override
	{
		m_Callback->Call(cLuaState::Nil, a_ErrorMsg);
	}


protected:

	/** The callback to call when the request finishes. */
	cLuaState::cCallbackPtr m_Callback;

	/** The accumulator for the partial body data, so that OnBodyFinished() can send the entire thing at once. */
	AString m_ResponseBody;

	/** Accumulator for all the headers to be reported in the combined callback. */
	AStringMap m_Headers;
};





/** Common code shared among the cUrlClient request methods.
a_Method is the method name to be used in the request.
a_UrlStackIdx is the position on the Lua stack of the Url parameter. */
static int tolua_cUrlClient_Request_Common(lua_State * a_LuaState, const AString & a_Method, int a_UrlStackIdx)
{
	// Check params:
	cLuaState L(a_LuaState);
	if (!L.CheckParamString(a_UrlStackIdx))
	{
		return 0;
	}

	// Read params:
	AString url, requestBody;
	AStringMap headers, options;
	cLuaState::cTableRefPtr callbacks;
	cLuaState::cCallbackPtr onCompleteBodyCallback;
	if (!L.GetStackValues(a_UrlStackIdx, url))
	{
		return L.ApiParamError("Cannot read URL parameter at idx %d", a_UrlStackIdx);
	}
	cUrlClient::cCallbacksPtr urlClientCallbacks;
	if (lua_istable(L, a_UrlStackIdx + 1))
	{
		if (!L.GetStackValue(a_UrlStackIdx + 1, callbacks))
		{
			return L.ApiParamError("Cannot read the CallbacksTable parameter at idx %d", a_UrlStackIdx + 1);
		}
		urlClientCallbacks = std::make_unique<cFullUrlClientCallbacks>(std::move(callbacks));
	}
	else if (lua_isfunction(L, a_UrlStackIdx + 1))
	{
		if (!L.GetStackValue(a_UrlStackIdx + 1, onCompleteBodyCallback))
		{
			return L.ApiParamError("Cannot read the CallbackFn parameter at idx %d", a_UrlStackIdx + 1);
		}
		urlClientCallbacks = std::make_unique<cSimpleUrlClientCallbacks>(std::move(onCompleteBodyCallback));
	}
	else
	{
		L.ApiParamError("Invalid Callbacks parameter at %d, expected a table or function, got %s", a_UrlStackIdx + 1, L.GetTypeText(a_UrlStackIdx + 1).c_str());
	}
	if (!L.GetStackValues(a_UrlStackIdx + 2, cLuaState::cOptionalParam<AStringMap>(headers), cLuaState::cOptionalParam<AString>(requestBody), cLuaState::cOptionalParam<AStringMap>(options)))
	{
		L.ApiParamError("Cannot read the Header, Body or Options parameter at idx %d, %d, %d.", a_UrlStackIdx + 2, a_UrlStackIdx + 3, a_UrlStackIdx + 4);
	}

	// Make the request:
	auto res = cUrlClient::Request(a_Method, url, std::move(urlClientCallbacks), std::move(headers), std::move(requestBody), std::move(options));
	if (!res.first)
	{
		L.Push(false);
		L.Push(res.second);
		return 2;
	}
	L.Push(true);
	return 1;
}





/** Implements cUrlClient:Get() using cUrlClient::Request(). */
static int tolua_cUrlClient_Delete(lua_State * a_LuaState)
{
	/* Function signatures:
	cUrlClient:Delete(URL, {CallbacksFnTable}, [{HeadersMapTable}], [RequestBody], [{OptionsMapTable}]) -> true / false + string
	cUrlClient:Delete(URL, OnCompleteBodyCallback, [{HeadersMapTable}], [RequestBody], [{OptionsMapTable}]) -> true / false + string
	*/

	return tolua_cUrlClient_Request_Common(a_LuaState, "DELETE", 2);
}





/** Implements cUrlClient:Get() using cUrlClient::Request(). */
static int tolua_cUrlClient_Get(lua_State * a_LuaState)
{
	/* Function signatures:
	cUrlClient:Get(URL, {CallbacksFnTable}, [{HeadersMapTable}], [RequestBody], [{OptionsMapTable}]) -> true / false + string
	cUrlClient:Get(URL, OnCompleteBodyCallback, [{HeadersMapTable}], [RequestBody], [{OptionsMapTable}]) -> true / false + string
	*/

	return tolua_cUrlClient_Request_Common(a_LuaState, "GET", 2);
}





/** Implements cUrlClient:Post() using cUrlClient::Request(). */
static int tolua_cUrlClient_Post(lua_State * a_LuaState)
{
	/* Function signatures:
	cUrlClient:Post(URL, {CallbacksFnTable}, [{HeadersMapTable}], [RequestBody], [{OptionsMapTable}]) -> true / false + string
	cUrlClient:Post(URL, OnCompleteBodyCallback, [{HeadersMapTable}], [RequestBody], [{OptionsMapTable}]) -> true / false + string
	*/

	return tolua_cUrlClient_Request_Common(a_LuaState, "POST", 2);
}





/** Implements cUrlClient:Put() using cUrlClient::Request(). */
static int tolua_cUrlClient_Put(lua_State * a_LuaState)
{
	/* Function signatures:
	cUrlClient:Put(URL, {CallbacksFnTable}, [{HeadersMapTable}], [RequestBody], [{OptionsMapTable}]) -> true / false + string
	cUrlClient:Put(URL, OnCompleteBodyCallback, [{HeadersMapTable}], [RequestBody], [{OptionsMapTable}]) -> true / false + string
	*/

	return tolua_cUrlClient_Request_Common(a_LuaState, "PUT", 2);
}





/** Binds cUrlClient::Request(). */
static int tolua_cUrlClient_Request(lua_State * a_LuaState)
{
	/* Function signatures:
	cUrlClient:Request(Method, URL, {CallbacksFnTable}, [{HeadersMapTable}], [RequestBody], [{OptionsMapTable}]) -> true / false + string
	cUrlClient:Request(Method, URL, OnCompleteBodyCallback, [{HeadersMapTable}], [RequestBody], [{OptionsMapTable}]) -> true / false + string
	*/

	// Check that the Method param is a string:
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamStaticSelf("cUrlClient") ||
		!L.CheckParamString(2))
	{
		return 0;
	}

	// Redirect the rest to the common code:
	AString method;
	if (!L.GetStackValue(2, method))
	{
		L.ApiParamError("Cannot read the Method parameter");
	}
	return tolua_cUrlClient_Request_Common(a_LuaState, method, 3);
}





////////////////////////////////////////////////////////////////////////////////
// Register the bindings:

void cManualBindings::BindNetwork(lua_State * tolua_S)
{
	// Create the cNetwork API classes:
	tolua_usertype(tolua_S, "cNetwork");
	tolua_usertype(tolua_S, "cServerHandle");
	tolua_usertype(tolua_S, "cTCPLink");
	tolua_usertype(tolua_S, "cUDPEndpoint");
	tolua_usertype(tolua_S, "cUrlClient");
	tolua_cclass(tolua_S, "cNetwork",      "cNetwork",      "", nullptr);
	tolua_cclass(tolua_S, "cServerHandle", "cServerHandle", "", tolua_collect_cServerHandle);
	tolua_cclass(tolua_S, "cTCPLink",      "cTCPLink",      "", nullptr);
	tolua_cclass(tolua_S, "cUDPEndpoint",  "cUDPEndpoint",  "", tolua_collect_cUDPEndpoint);
	tolua_cclass(tolua_S, "cUrlClient",    "cUrlClient",    "", nullptr);

	// Fill in the functions (alpha-sorted):
	tolua_beginmodule(tolua_S, "cNetwork");
		tolua_function(tolua_S, "Connect",              tolua_cNetwork_Connect);
		tolua_function(tolua_S, "CreateUDPEndpoint",    tolua_cNetwork_CreateUDPEndpoint);
		tolua_function(tolua_S, "EnumLocalIPAddresses", tolua_cNetwork_EnumLocalIPAddresses);
		tolua_function(tolua_S, "HostnameToIP",         tolua_cNetwork_HostnameToIP);
		tolua_function(tolua_S, "IPToHostname",         tolua_cNetwork_IPToHostname);
		tolua_function(tolua_S, "Listen",               tolua_cNetwork_Listen);
	tolua_endmodule(tolua_S);

	tolua_beginmodule(tolua_S, "cServerHandle");
		tolua_function(tolua_S, "Close",       tolua_cServerHandle_Close);
		tolua_function(tolua_S, "IsListening", tolua_cServerHandle_IsListening);
	tolua_endmodule(tolua_S);

	tolua_beginmodule(tolua_S, "cTCPLink");
		tolua_function(tolua_S, "Close",          tolua_cTCPLink_Close);
		tolua_function(tolua_S, "GetLocalIP",     tolua_cTCPLink_GetLocalIP);
		tolua_function(tolua_S, "GetLocalPort",   tolua_cTCPLink_GetLocalPort);
		tolua_function(tolua_S, "GetRemoteIP",    tolua_cTCPLink_GetRemoteIP);
		tolua_function(tolua_S, "GetRemotePort",  tolua_cTCPLink_GetRemotePort);
		tolua_function(tolua_S, "Send",           tolua_cTCPLink_Send);
		tolua_function(tolua_S, "Shutdown",       tolua_cTCPLink_Shutdown);
		tolua_function(tolua_S, "StartTLSClient", tolua_cTCPLink_StartTLSClient);
		tolua_function(tolua_S, "StartTLSServer", tolua_cTCPLink_StartTLSServer);
	tolua_endmodule(tolua_S);

	tolua_beginmodule(tolua_S, "cUDPEndpoint");
		tolua_function(tolua_S, "Close",            tolua_cUDPEndpoint_Close);
		tolua_function(tolua_S, "EnableBroadcasts", tolua_cUDPEndpoint_EnableBroadcasts);
		tolua_function(tolua_S, "GetPort",          tolua_cUDPEndpoint_GetPort);
		tolua_function(tolua_S, "IsOpen",           tolua_cUDPEndpoint_IsOpen);
		tolua_function(tolua_S, "Send",             tolua_cUDPEndpoint_Send);
	tolua_endmodule(tolua_S);

	tolua_beginmodule(tolua_S, "cUrlClient");
		tolua_function(tolua_S, "Delete",  tolua_cUrlClient_Delete);
		tolua_function(tolua_S, "Get",     tolua_cUrlClient_Get);
		tolua_function(tolua_S, "Post",    tolua_cUrlClient_Post);
		tolua_function(tolua_S, "Put",     tolua_cUrlClient_Put);
		tolua_function(tolua_S, "Request", tolua_cUrlClient_Request);
	tolua_endmodule(tolua_S);
}




