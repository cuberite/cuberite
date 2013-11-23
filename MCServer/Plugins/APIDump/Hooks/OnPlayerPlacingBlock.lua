return
{
	HOOK_PLAYER_PLACING_BLOCK =
	{
		CalledWhen = "Just before a player places a block. Plugin may override / refuse.",
		DefaultFnName = "OnPlayerPlacingBlock",  -- also used as pagename
		Desc = [[
			This hook is called just before a {{cPlayer|player}} places a block in the {{cWorld|world}}. The
			block is not yet placed, plugins may choose to override the default behavior or refuse the placement
			at all.</p>
			<p>
			Note that the client already expects that the block has been placed. For that reason, if a plugin
			refuses the placement, MCServer sends the old block at the provided coords to the client.</p>
			<p>
			Use the {{cPlayer}}:GetWorld() function to get the world to which the block belongs.</p>
			<p>
			See also the {{OnPlayerPlacedBlock|HOOK_PLAYER_PLACED_BLOCK}} hook for a similar hook called after
			the placement.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player who is placing the block" },
			{ Name = "BlockX", Type = "number", Notes = "X-coord of the block" },
			{ Name = "BlockY", Type = "number", Notes = "Y-coord of the block" },
			{ Name = "BlockZ", Type = "number", Notes = "Z-coord of the block" },
			{ Name = "BlockFace", Type = "number", Notes = "Face of the existing block upon which the player is interacting. One of the BLOCK_FACE_ constants" },
			{ Name = "CursorX", Type = "number", Notes = "X-coord of the cursor within the block face (0 .. 15)" },
			{ Name = "CursorY", Type = "number", Notes = "Y-coord of the cursor within the block face (0 .. 15)" },
			{ Name = "CursorZ", Type = "number", Notes = "Z-coord of the cursor within the block face (0 .. 15)" },
			{ Name = "BlockType", Type = "BLOCKTYPE", Notes = "The block type of the block" },
			{ Name = "BlockMeta", Type = "NIBBLETYPE", Notes = "The block meta of the block" },
		},
		Returns = [[
			If this function returns false or no value, MCServer calls other plugins with the same event and
			finally places the block and removes the corresponding item from player's inventory. If this
			function returns true, no other plugin is called for this event, MCServer sends the old block at
			the specified coords to the client and drops the packet.
		]],
	},  -- HOOK_PLAYER_PLACING_BLOCK
}





