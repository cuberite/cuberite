return
{
	HOOK_MONSTER_MOVED =
	{
		CalledWhen = "After a monster has moved to a new position.",
		DefaultFnName = "OnMonsterMoved",  -- also used as pagename
		Desc = [[
            This hook is called after a {{cMonster|monster}} has moved, and the
            entity move packet has been broadcast to clients.</p>
		]],
		Params =
		{
			{ Name = "Monster", Type = "{{cMonster}}", Notes = "The monster who has moved. The object already has the new position stored in it." },
			{ Name = "OldPosition", Type = "{{Vector3d}}", Notes = "The old position." },
			{ Name = "NewPosition", Type = "{{Vector3d}}", Notes = "The new position." },
		},
		Returns = [[
            If the function returns false or no value, other plugins' callbacks are called. If the function
            returns true, no other callbacks are called for this event. There is no overridable behavior.
		]],
	},  -- HOOK_MONSTER_MOVED
}





