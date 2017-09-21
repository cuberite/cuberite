return
{
	HOOK_PLAYER_EDITING_BOOK =
	{
		CalledWhen = "A player is editing a book.",
		DefaultFnName = "OnPlayerEditingBook",  -- also used as pagename
		Desc = [[
			This hook is called whenever a {{cPlayer|player}} is editing a book.
			See also the {{OnPlayerEditingBook|HOOK_PLAYER_EDITED_BOOK}} hook for a similar hook, is called when a
			player has edited a book.
		]],
		Params =
		{
			{ Name = "Player", Type = "cPlayer", Notes = "The player that is editing the book" },
			{ Name = "OriginalContent", Type = "cBookContent", Notes = "Contains the original content of the book" },
			{ Name = "NewContent", Type = "cBookContent", Notes = "Contains the new content of the book" },
			{ Name = "IsSigned", Type = "boolean", Notes = "Player is signing the book" },
		},
		Returns = [[
			If the function returns false or no value, Cuberite calls other plugins with this event. If the function returns true,
			no other plugin's callback is called and the editing of the book is denied.
		]],
	},  -- HOOK_PLAYER_EDITING_BOOK
}




