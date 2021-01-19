return
{
	HOOK_PAWN_FELL =
	{
		CalledWhen = "A pawn has fallen onto a block",
		DefaultFnName = "OnPawnFell",  -- also used as pagename
		Desc = [[
			This hook is called whenever a cPawn has fallen onto a block.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player from whom the packet was received" },
			{ Name = "Animation", Type = "number", Notes = "The kind of animation" },
		},
		Returns = [[
			If the function returns false or no value, the next plugin's callback is called. Afterwards, the
			server broadcasts the animation packet to all nearby clients. If the function returns true, no other
			callback is called for this event and the packet is not broadcasted.
		]],
	},  -- HOOK_PAWN_FELL
}




