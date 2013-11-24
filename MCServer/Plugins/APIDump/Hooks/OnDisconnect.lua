return
{
	HOOK_DISCONNECT =
	{
		CalledWhen = "A player has explicitly disconnected.",
		DefaultFnName = "OnDisconnect",  -- also used as pagename
		Desc = [[
			This hook is called when a client sends the disconnect packet and is about to be disconnected from
			the server.</p>
			<p>
			Note that this callback is not called if the client drops the connection or is kicked by the
			server.</p>
			<p>
			FIXME: There is no callback for "client destroying" that would be called in all circumstances.</p>
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player who has disconnected" },
			{ Name = "Reason", Type = "string", Notes = "The reason that the client has sent in the disconnect packet" },
		},
		Returns = [[
			If the function returns false or no value, MCServer calls other plugins' callbacks for this event
			and finally broadcasts a disconnect message to the player's world. If the function returns true, no
			other plugins are called for this event and the disconnect message is not broadcast. In either case,
			the player is disconnected.
		]],
	},  -- HOOK_DISCONNECT
}




