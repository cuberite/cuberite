return
{
	HOOK_PLAYER_OPENING_WINDOW =
	{
		CalledWhen = "Called when a player is about to open a window",
		DefaultFnName = "OnPlayerOpeningWindow",  -- also used as pagename
		Desc = [[
			This hook is called when a player is about to open a window, e.g. when they click on a chest or a furnace.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player who is opening the window" },
			{ Name = "Window", Type = "{{cWindow}}", Notes = "The window that is being opened" },
		},
		Returns = [[
			If the function returns false or no value, the next plugin's callback is called, and finally
			Cuberite will process the opening window. If the function returns true, no other callback is called for this event.
		]],
	},  -- HOOK_PLAYER_OPENING_WINDOW
}
