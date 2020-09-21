return
{
	HOOK_DROPSPENSE =
	{
		CalledWhen = "A dispenser or dropper is about to dispense/drop an item.",
		DefaultFnName = "OnDropSpense",  -- also used as pagename
		Desc = [[
			This callback is called whenever a {{cDropSpenserEntity|dropspenser}} dropspenses an {{cItem|item}}. A plugin may decide to disallow 
      the move by returning true.
		]],
		Params =
		{
			{ Name = "World", Type = "{{cWorld}}", Notes = "World where the dropspenser resides" },
			{ Name = "DropSpenser", Type = "{{cDropSpenserEntity}}", Notes = "The dropspenser that is pulling the item" },
			{ Name = "SlotNum", Type = "number", Notes = "The slot of the dropspensed item in the dropspenser's {{cItemGrid|internal storage}}" },
		},
		Returns = [[
			If the function returns false or no value, the next plugin's callback is called. If the function
			returns true, no other callback is called for this event and the dropspenser will not dropspense the item.
		]],
	},  -- HOOK_DROPSPENSE
}




