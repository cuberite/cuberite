return
{
	HOOK_PLAYER_EDITED_BOOK =
	{
		CalledWhen = "A player has edited a book.",
		DefaultFnName = "OnPlayerEditedBook",  -- also used as pagename
		Desc = [[
			This hook is called whenever a {{cPlayer|player}} has edited a book.
			See also the {{OnPlayerEditingBook|HOOK_PLAYER_EDITING_BOOK}} hook for a similar hook, is called when a
			player is editing a book.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "The player that edited the book" },
			{ Name = "BookContent", Type = "{{cBookContent}}", Notes = "The class that contains the current info of the book" },
			{ Name = "IsSigned", Type = "boolean", Notes = "Player has signed the book" },
		},
		Returns = [[
			If the function returns false or no value, Cuberite calls other plugins with this event. If the
			function returns true, no other plugin is called for this event.</p>
		]],
	},  -- HOOK_PLAYER_EDITED_BOOK
}




