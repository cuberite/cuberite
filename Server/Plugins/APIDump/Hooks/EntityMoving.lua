return
{
	HOOK_ENTITY_MOVING =
	{
		CalledWhen = "After a entity has moved to a new position.",
		DefaultFnName = "OnEntityMoving",  -- also used as pagename
		Desc = [[
            This hook is called while a {{cEntity|entity}} is moving. Can be used
            to cancel the movement. This hook is NOT called, when a entity is attached to something.
		]],
		Params =
		{
			{ Name = "Entity", Type = "{{cEntity}}", Notes = "The entity that is moving." },
			{ Name = "OldPosition", Type = "{{Vector3d}}", Notes = "The old position." },
			{ Name = "NewPosition", Type = "{{Vector3d}}", Notes = "The new position." },
		},
		Returns = [[
            If the function returns false or no value, other plugins' callbacks are called. If the function
            returns true, no other callbacks are called for this event and the entity isn't moved.
		]],
	},  -- HOOK_ENTITY_MOVING
}





