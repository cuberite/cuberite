return
{
	HOOK_PLAYER_CROUCHED =
	{
		CalledWhen = "Player crouched in the tick being currently processed.",
		DefaultFnName = "OnPlayerCrouched",  -- also used as pagename
		Desc = [[
			This function is called in each server tick for each {{cPlayer|player}} that has crouched.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player who has crouched." },
		},
		Returns = [[
			If the function returns false or no value, other plugins callbacks are called.</p>
		]],
	},  -- HOOK_PLAYER_CROUCHED
}