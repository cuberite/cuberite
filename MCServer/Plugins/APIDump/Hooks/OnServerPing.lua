 (cClientHandle & a_ClientHandle, AString & a_ServerDescription, int & a_OnlinePlayersCount, int & a_MaxPlayersCount, AString & a_Favicon);
 
return
{
	HOOK_SERVER_PING =
	{
		CalledWhen = "Client pings the server from the server list.",
		DefaultFnName = "OnServerPing",  -- also used as pagename
		Desc = [[
			A plugin may implement an OnServerPing() function and register it as a Hook to process pings from
			clients in the server server list.
		]],
		Params = {
			{ Name = "ClientHandle", Type = "{{cClientHandle}}", Notes = "The client handle that pinged the server" },
			{ Name = "ServerDescription", Type = "string", Notes = "The server description" },
			{ Name = "OnlinePlayersCount", Type = "number", Notes = "The number of players currently on the server" },
			{ Name = "MaxPlayersCount", Type = "number", Notes = "The current player cap for the server" },
			{ Name = "Favicon", Type = "string", Notes = "The base64 encoded favicon to be displayed in the server list for compatible clients" },
		},
		Returns = [[
			The plugin may return a boolean of whether to respond to the client that pinged. 
		]],
	},  -- HOOK_SERVER_PING
}
