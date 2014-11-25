return
{
	HOOK_PLAYER_LEFT_CLICK =
	{
		CalledWhen = "A client do a left click. Plugin may override / refuse.",
		DefaultFnName = "OnPlayerLeftClick",  -- also used as pagename
		Desc = [[
			This hook is called when a player do a left click to a specific block or the air.</p>
			<p>
			Plugins may refuse the default processing for the packet, causing MCServer to behave as if the
			packet has never arrived. This may, however, create inconsistencies in the client - the client may
			think that they broke a block, while the server didn't process the breaking, etc. For this reason,
			if a plugin refuses the processing, MCServer sends the block specified in the packet back to the
			client (as if placed anew).</p>
			<p>
			If BlockFace is BLOCK_FACE_NONE, the client interacts with the air and not with a specific block.
			In this case, BlockX/Y/Z are 0.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player whose client sent the packet" },
			{ Name = "BlockX", Type = "number", Notes = "X-coord of the block" },
			{ Name = "BlockY", Type = "number", Notes = "Y-coord of the block" },
			{ Name = "BlockZ", Type = "number", Notes = "Z-coord of the block" },
			{ Name = "BlockFace", Type = "number", Notes = "Face of the block upon which the player interacted. One of the BLOCK_FACE_ constants" },
		},
		Returns = [[
			If the function returns false or no value, MCServer calls other plugins' callbacks and finally break
			the block.</p>
			<p>
			If the function returns true, no other plugins are called, processing is halted.
			MCServer sends the block specified in the coords back to the client. The packet is dropped.
		]],
	},  -- HOOK_PLAYER_LEFT_CLICK
}





