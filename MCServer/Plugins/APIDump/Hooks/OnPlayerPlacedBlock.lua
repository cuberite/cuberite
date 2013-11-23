return
{
	HOOK_PLAYER_PLACED_BLOCK =
	{
		CalledWhen = "After a player has placed a block. Notification only.",
		DefaultFnName = "OnPlayerPlacedBlock",  -- also used as pagename
		Desc = [[
			This hook is called after a {{cPlayer|player}} has placed a block in the {{cWorld|world}}. The block
			is already added to the world and the corresponding item removed from player's
			{{cInventory|inventory}}.</p>
			<p>
			Use the {{cPlayer}}:GetWorld() function to get the world to which the block belongs.</p>
			<p>
			See also the {{OnPlayerPlacingBlock|HOOK_PLAYER_PLACING_BLOCK}} hook for a similar hook called
			before the placement.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player who placed the block" },
			{ Name = "BlockX", Type = "number", Notes = "X-coord of the block" },
			{ Name = "BlockY", Type = "number", Notes = "Y-coord of the block" },
			{ Name = "BlockZ", Type = "number", Notes = "Z-coord of the block" },
			{ Name = "BlockFace", Type = "number", Notes = "Face of the existing block upon which the player interacted. One of the BLOCK_FACE_ constants" },
			{ Name = "CursorX", Type = "number", Notes = "X-coord of the cursor within the block face (0 .. 15)" },
			{ Name = "CursorY", Type = "number", Notes = "Y-coord of the cursor within the block face (0 .. 15)" },
			{ Name = "CursorZ", Type = "number", Notes = "Z-coord of the cursor within the block face (0 .. 15)" },
			{ Name = "BlockType", Type = "BLOCKTYPE", Notes = "The block type of the block" },
			{ Name = "BlockMeta", Type = "NIBBLETYPE", Notes = "The block meta of the block" },
		},
		Returns = [[
			If this function returns false or no value, MCServer calls other plugins with the same event. If
			this function returns true, no other plugin is called for this event.
		]],
	},  -- HOOK_PLAYER_PLACED_BLOCK
}





