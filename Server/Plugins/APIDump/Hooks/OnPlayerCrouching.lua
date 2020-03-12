return
{
	HOOK_PLAYER_CROUCHING =
	{
		CalledWhen = "Player tried to crouch in the tick being currently processed. Plugin may refuse crouching.",
		DefaultFnName = "OnPlayerCrouching",  -- also used as pagename
		Desc = [[
			This function is called in each server tick for each {{cPlayer|player}} that has crouched. Plugins may refuse the crouching.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player who has ." },
			{ Name = "IsCrouched", Type = "{{boolean}}", Notes = "Is the player crouched or standing?" },
		},
		Returns = [[
			If the function returns true, crouching is prohibited.</p>
			<p>
			If the function returns false or no value, other plugins' callbacks are called and the position of the player is broadcasted.</p>
		]],
	},  -- HOOK_PLAYER_CROUCHING  
}
