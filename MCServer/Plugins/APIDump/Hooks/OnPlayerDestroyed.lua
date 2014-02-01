return
{
	HOOK_PLAYER_DESTROYED =
	{
		CalledWhen = "A player is about to get destroyed.",
		DefaultFnName = "OnPlayerDestroyed",  -- also used as pagename
		Desc = [[
			This function is called when a {{cPlayer|player}} is about to get destroyed. But the player isn't already destroyed.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The destroyed player" },
		},
		Returns = [[
			It's only for notification. Can't be returned.
		]],
	},  -- HOOK_PLAYER_DESTROYED
}





