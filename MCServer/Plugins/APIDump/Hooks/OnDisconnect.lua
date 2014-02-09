return
{
	HOOK_DISCONNECT =
	{
		CalledWhen = "A player has explicitly disconnected.",
		DefaultFnName = "OnDisconnect",  -- also used as pagename
		Desc = [[
			This hook is called when a client is about to be disconnected from the server, for whatever reason.
			
			<p><b>Note that this hook will be removed after <1.7 protocol support is removed, as it was originally a hook for
			the client sending the server a disconnect packet, which no longer happens.</b></p>
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player who has disconnected" },
			{ Name = "Reason", Type = "string", Notes = "The reason that the client has sent in the disconnect packet" },
		},
		Returns = [[
			If the function returns false or no value, MCServer calls other plugins' callbacks for this event.
			If the function returns true, no other plugins are called for this event. In either case,
			the player is disconnected.
		]],
	},  -- HOOK_DISCONNECT
}




