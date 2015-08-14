
// ManualBindings_Network.cpp

// Implements the cNetwork-related API bindings for Lua

#include "Globals.h"
#include "LuaTCPLink.h"
#include "ManualBindings.h"
#include "tolua++/include/tolua++.h"
#include "LuaState.h"
#include "LuaTCPLink.h"
#include "LuaNameLookup.h"
#include "LuaServerHandle.h"
#include "LuaUDPEndpoint.h"





////////////////////////////////////////////////////////////////////////////////
// cNetwork API functions:

/** Binds cNetwork::Connect */
static int tolua_cNetwork_Connect(lua_State * L)
{
	// Function signature:
	// cNetwork:Connect(Host, Port, Callbacks) -> bool

	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cNetwork") ||
		!S.CheckParamString(2) ||
		!S.CheckParamNumber(3) ||
		!S.CheckParamTable(4) ||
		!S.CheckParamEnd(5)
	)
	{
		return 0;
	}
	
	// Get the plugin instance:
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(L);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		S.Push(false);
		return 1;
	}

	// Read the params:
	AString Host;
	int Port = 0;
	S.GetStackValues(2, Host, Port);

	// Check validity:
	if ((Port < 0) || (Port > 65535))
	{
		LOGWARNING("cNetwork:Connect() called with invalid port (%d), failing the request.", Port);
		S.Push(false);
		return 1;
	}

	// Create the LuaTCPLink glue class:
	auto Link = std::make_shared<cLuaTCPLink>(*Plugin, 4);

	// Try to connect:
	bool res = cNetwork::Connect(Host, static_cast<UInt16>(Port), Link, Link);
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
		!S.CheckParamUserTable(1, "cNetwork") ||
		!S.CheckParamNumber(2) ||
		!S.CheckParamTable(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the plugin instance:
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(L);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		S.Push(false);
		return 1;
	}

	// Read the params:
	UInt16 Port;

	// Check validity:
	if (!S.GetStackValues(2, Port))
	{
		LOGWARNING("cNetwork:CreateUDPEndpoint() called with invalid port, failing the request.");
		S.Push(false);
		return 1;
	}

	// Create the LuaUDPEndpoint glue class:
	auto Endpoint = std::make_shared<cLuaUDPEndpoint>(*Plugin, 3);
	Endpoint->Open(Port, Endpoint);

	// Register the endpoint to be garbage-collected by Lua:
	tolua_pushusertype(L, Endpoint.get(), "cUDPEndpoint");
	tolua_register_gc(L, lua_gettop(L));

	// Return the endpoint object:
	S.Push(Endpoint.get());
	return 1;
}





/** Binds cNetwork::EnumLocalIPAddresses */
static int tolua_cNetwork_EnumLocalIPAddresses(lua_State * L)
{
	// Function signature:
	// cNetwork:EnumLocalIPAddresses() -> {string, ...}

	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cNetwork") ||
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
		!S.CheckParamUserTable(1, "cNetwork") ||
		!S.CheckParamString(2) ||
		!S.CheckParamTable(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the plugin instance:
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(L);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		S.Push(false);
		return 1;
	}

	// Read the params:
	AString Host;
	S.GetStackValue(2, Host);

	// Try to look up:
	bool res = cNetwork::HostnameToIP(Host, std::make_shared<cLuaNameLookup>(Host, *Plugin, 3));
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
		!S.CheckParamUserTable(1, "cNetwork") ||
		!S.CheckParamString(2) ||
		!S.CheckParamTable(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the plugin instance:
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(L);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		S.Push(false);
		return 1;
	}

	// Read the params:
	AString Host;
	S.GetStackValue(2, Host);

	// Try to look up:
	bool res = cNetwork::IPToHostName(Host, std::make_shared<cLuaNameLookup>(Host, *Plugin, 3));
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
		!S.CheckParamUserTable(1, "cNetwork") ||
		!S.CheckParamNumber(2) ||
		!S.CheckParamTable(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Get the plugin instance:
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(L);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		S.Push(false);
		return 1;
	}

	// Read the params:
	int Port;
	S.GetStackValues(2, Port);
	if ((Port < 0) || (Port > 65535))
	{
		LOGWARNING("cNetwork:Listen() called with invalid port (%d), failing the request.", Port);
		S.Push(false);
		return 1;
	}
	UInt16 Port16 = static_cast<UInt16>(Port);

	// Create the LuaTCPLink glue class:
	auto Srv = std::make_shared<cLuaServerHandle>(Port16, *Plugin, 3);

	// Listen:
	Srv->SetServerHandle(cNetwork::Listen(Port16, Srv), Srv);

	// Register the server to be garbage-collected by Lua:
	tolua_pushusertype(L, Srv.get(), "cServerHandle");
	tolua_register_gc(L, lua_gettop(L));

	// Return the server handle wrapper:
	S.Push(Srv.get());
	return 1;
}





////////////////////////////////////////////////////////////////////////////////
// cServerHandle bindings (routed through cLuaServerHandle):

/** Called when Lua destroys the object instance.
Close the server and let it deallocate on its own (it's in a SharedPtr). */
static int tolua_collect_cServerHandle(lua_State * L)
{
	cLuaServerHandle * Srv = static_cast<cLuaServerHandle *>(tolua_tousertype(L, 1, nullptr));
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
		!S.CheckParamUserType(1, "cServerHandle") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the server handle:
	cLuaServerHandle * Srv;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cServerHandle:Close(): invalid server handle object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Srv = *static_cast<cLuaServerHandle **>(lua_touserdata(L, 1));

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
		!S.CheckParamUserType(1, "cServerHandle") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the server handle:
	cLuaServerHandle * Srv;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cServerHandle:IsListening(): invalid server handle object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Srv = *static_cast<cLuaServerHandle **>(lua_touserdata(L, 1));

	// Close it:
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
		!S.CheckParamUserType(1, "cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the link:
	cLuaTCPLink * Link;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cTCPLink:Close(): invalid link object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));

	// CLose the link:
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
		!S.CheckParamUserType(1, "cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the link:
	cLuaTCPLink * Link;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cTCPLink:GetLocalIP(): invalid link object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));

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
		!S.CheckParamUserType(1, "cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the link:
	cLuaTCPLink * Link;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cTCPLink:GetLocalPort(): invalid link object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));

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
		!S.CheckParamUserType(1, "cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the link:
	cLuaTCPLink * Link;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cTCPLink:GetRemoteIP(): invalid link object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));

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
		!S.CheckParamUserType(1, "cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the link:
	cLuaTCPLink * Link;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cTCPLink:GetRemotePort(): invalid link object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));

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
		!S.CheckParamUserType(1, "cTCPLink") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the link:
	cLuaTCPLink * Link;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cTCPLink:Send(): invalid link object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));

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
		!S.CheckParamUserType(1, "cTCPLink") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the link:
	cLuaTCPLink * Link;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cTCPLink:Shutdown(): invalid link object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));

	// Shutdown the link:
	Link->Shutdown();
	return 0;
}





/** Binds cLuaTCPLink::StartTLSClient */
static int tolua_cTCPLink_StartTLSClient(lua_State * L)
{
	// Function signature:
	// LinkInstance:StartTLSClient(OwnCert, OwnPrivKey, OwnPrivKeyPassword) -> [true] or [nil, ErrMsg]

	cLuaState S(L);
	if (
		!S.CheckParamUserType(1, "cTCPLink") ||
		!S.CheckParamString(2, 4) ||
		!S.CheckParamEnd(5)
	)
	{
		return 0;
	}
	
	// Get the link:
	cLuaTCPLink * Link;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cTCPLink:StartTLSClient(): invalid link object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));

	// Read the params:
	AString OwnCert, OwnPrivKey, OwnPrivKeyPassword;
	S.GetStackValues(2, OwnCert, OwnPrivKey, OwnPrivKeyPassword);

	// Start the TLS handshake:
	AString res = Link->StartTLSClient(OwnCert, OwnPrivKey, OwnPrivKeyPassword);
	if (!res.empty())
	{
		S.PushNil();
		S.Push(Printf("Cannot start TLS on link to %s:%d: %s", Link->GetRemoteIP().c_str(), Link->GetRemotePort(), res.c_str()));
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
		!S.CheckParamUserType(1, "cTCPLink") ||
		!S.CheckParamString(2, 4) ||
		// Param 5 is optional, don't check
		!S.CheckParamEnd(6)
	)
	{
		return 0;
	}
	
	// Get the link:
	cLuaTCPLink * Link;
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cTCPLink:StartTLSServer(): invalid link object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	Link = *static_cast<cLuaTCPLink **>(lua_touserdata(L, 1));

	// Read the params:
	AString OwnCert, OwnPrivKey, OwnPrivKeyPassword, StartTLSData;
	S.GetStackValues(2, OwnCert, OwnPrivKey, OwnPrivKeyPassword, StartTLSData);

	// Start the TLS handshake:
	AString res = Link->StartTLSServer(OwnCert, OwnPrivKey, OwnPrivKeyPassword, StartTLSData);
	if (!res.empty())
	{
		S.PushNil();
		S.Push(Printf("Cannot start TLS on link to %s:%d: %s", Link->GetRemoteIP().c_str(), Link->GetRemotePort(), res.c_str()));
		return 2;
	}
	return 1;
}





////////////////////////////////////////////////////////////////////////////////
// cUDPEndpoint bindings (routed through cLuaUDPEndpoint):

/** Called when Lua destroys the object instance.
Close the endpoint and let it deallocate on its own (it's in a SharedPtr). */
static int tolua_collect_cUDPEndpoint(lua_State * L)
{
	LOGD("Lua: Collecting cUDPEndpoint");
	cLuaUDPEndpoint * Endpoint = static_cast<cLuaUDPEndpoint *>(tolua_tousertype(L, 1, nullptr));
	Endpoint->Release();
	return 0;
}





/** Binds cLuaUDPEndpoint::Close */
static int tolua_cUDPEndpoint_Close(lua_State * L)
{
	// Function signature:
	// EndpointInstance:Close()

	cLuaState S(L);
	if (
		!S.CheckParamUserType(1, "cUDPEndpoint") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the endpoint:
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cUDPEndpoint:Close(): invalid endpoint object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	auto Endpoint = *static_cast<cLuaUDPEndpoint **>(lua_touserdata(L, 1));

	// Close it:
	Endpoint->Close();
	return 0;
}





/** Binds cLuaUDPEndpoint::EnableBroadcasts */
static int tolua_cUDPEndpoint_EnableBroadcasts(lua_State * L)
{
	// Function signature:
	// EndpointInstance:EnableBroadcasts()

	cLuaState S(L);
	if (
		!S.CheckParamUserType(1, "cUDPEndpoint") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the endpoint:
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cUDPEndpoint:EnableBroadcasts(): invalid endpoint object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	auto Endpoint = *static_cast<cLuaUDPEndpoint **>(lua_touserdata(L, 1));

	// Enable the broadcasts:
	Endpoint->EnableBroadcasts();
	return 0;
}





/** Binds cLuaUDPEndpoint::GetPort */
static int tolua_cUDPEndpoint_GetPort(lua_State * L)
{
	// Function signature:
	// Endpoint:GetPort() -> number

	cLuaState S(L);
	if (
		!S.CheckParamUserType(1, "cUDPEndpoint") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the endpoint:
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cUDPEndpoint:GetPort(): invalid endpoint object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	auto Endpoint = *static_cast<cLuaUDPEndpoint **>(lua_touserdata(L, 1));

	// Get the Port:
	S.Push(Endpoint->GetPort());
	return 1;
}





/** Binds cLuaUDPEndpoint::IsOpen */
static int tolua_cUDPEndpoint_IsOpen(lua_State * L)
{
	// Function signature:
	// Endpoint:IsOpen() -> bool

	cLuaState S(L);
	if (
		!S.CheckParamUserType(1, "cUDPEndpoint") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	// Get the endpoint:
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cUDPEndpoint:IsListening(): invalid server handle object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	auto Endpoint = *static_cast<cLuaUDPEndpoint **>(lua_touserdata(L, 1));

	// Close it:
	S.Push(Endpoint->IsOpen());
	return 1;
}





/** Binds cLuaUDPEndpoint::Send */
static int tolua_cUDPEndpoint_Send(lua_State * L)
{
	// Function signature:
	// LinkInstance:Send(DataString)

	cLuaState S(L);
	if (
		!S.CheckParamUserType(1, "cUDPEndpoint") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamNumber(4) ||
		!S.CheckParamEnd(5)
	)
	{
		return 0;
	}
	
	// Get the link:
	if (lua_isnil(L, 1))
	{
		LOGWARNING("cUDPEndpoint:Send(): invalid endpoint object. Stack trace:");
		S.LogStackTrace();
		return 0;
	}
	auto Endpoint = *static_cast<cLuaUDPEndpoint **>(lua_touserdata(L, 1));

	// Get the data to send:
	AString Data, RemotePeer;
	int RemotePort = 0;
	S.GetStackValues(2, Data, RemotePeer, RemotePort);

	// Check the port:
	if ((RemotePort < 0) || (RemotePort > USHRT_MAX))
	{
		LOGWARNING("cUDPEndpoint:Send() called with invalid port (%d), failing.", RemotePort);
		S.LogStackTrace();
		S.Push(false);
		return 1;
	}

	// Send the data:
	S.Push(Endpoint->Send(Data, RemotePeer, static_cast<UInt16>(RemotePort)));
	return 1;
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
	tolua_cclass(tolua_S, "cNetwork",      "cNetwork",      "", nullptr);
	tolua_cclass(tolua_S, "cServerHandle", "cServerHandle", "", tolua_collect_cServerHandle);
	tolua_cclass(tolua_S, "cTCPLink",      "cTCPLink",      "", nullptr);
	tolua_cclass(tolua_S, "cUDPEndpoint",  "cUDPEndpoint",  "", tolua_collect_cUDPEndpoint);
	
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

}




