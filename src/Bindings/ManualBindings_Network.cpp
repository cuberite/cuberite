
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
	cPluginLua * Plugin = GetLuaPlugin(L);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		S.Push(false);
		return 1;
	}

	// Read the params:
	AString Host;
	int Port;
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
	cPluginLua * Plugin = GetLuaPlugin(L);
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
	cPluginLua * Plugin = GetLuaPlugin(L);
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
	// cNetwork:Listen(Port, Callbacks) -> bool

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
	cPluginLua * Plugin = GetLuaPlugin(L);
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
// cTCPLink bindings (routed through cLuaTCPLink):

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
// Register the bindings:

void ManualBindings::BindNetwork(lua_State * tolua_S)
{
	// Create the cNetwork API classes:
	tolua_usertype(tolua_S, "cNetwork");
	tolua_cclass(tolua_S, "cNetwork", "cNetwork", "", nullptr);
	tolua_usertype(tolua_S, "cTCPLink");
	tolua_cclass(tolua_S, "cTCPLink", "cTCPLink", "", nullptr);
	tolua_usertype(tolua_S, "cServerHandle");
	tolua_cclass(tolua_S, "cServerHandle", "cServerHandle", "", tolua_collect_cServerHandle);
	
	// Fill in the functions (alpha-sorted):
	tolua_beginmodule(tolua_S, "cNetwork");
		tolua_function(tolua_S, "Connect",      tolua_cNetwork_Connect);
		tolua_function(tolua_S, "HostnameToIP", tolua_cNetwork_HostnameToIP);
		tolua_function(tolua_S, "IPToHostname", tolua_cNetwork_IPToHostname);
		tolua_function(tolua_S, "Listen",       tolua_cNetwork_Listen);
	tolua_endmodule(tolua_S);

	tolua_beginmodule(tolua_S, "cTCPLink");
		tolua_function(tolua_S, "Send",          tolua_cTCPLink_Send);
		tolua_function(tolua_S, "GetLocalIP",    tolua_cTCPLink_GetLocalIP);
		tolua_function(tolua_S, "GetLocalPort",  tolua_cTCPLink_GetLocalPort);
		tolua_function(tolua_S, "GetRemoteIP",   tolua_cTCPLink_GetRemoteIP);
		tolua_function(tolua_S, "GetRemotePort", tolua_cTCPLink_GetRemotePort);
	tolua_endmodule(tolua_S);

	tolua_beginmodule(tolua_S, "cServerHandle");
		tolua_function(tolua_S, "Close",       tolua_cServerHandle_Close);
		tolua_function(tolua_S, "IsListening", tolua_cServerHandle_IsListening);
	tolua_endmodule(tolua_S);
}




