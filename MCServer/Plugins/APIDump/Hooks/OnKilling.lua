return
{
	HOOK_KILLING =
	{
		CalledWhen = "A player or a mob is dying.",
		DefaultFnName = "OnKilling",  -- also used as pagename
		Desc = [[
			This hook is called whenever a {{cPawn|pawn}}'s (a player's or a mob's) health reaches zero. This
			means that the pawn is about to be killed, unless a plugin "revives" them by setting their health
			back to a positive value.</p>
			<p>
			FIXME: There is no HOOK_KILLED notification hook yet; this is deliberate because HOOK_KILLED has
			been recently renamed to HOOK_KILLING, and plugins need to be updated. Once updated, the HOOK_KILLED
			notification will be implemented.
		]],
		Params =
		{
			{ Name = "Victim", Type = "{{cPawn}}", Notes = "The player or mob that is about to be killed" },
			{ Name = "Killer", Type = "{{cEntity}}", Notes = "The entity that has caused the victim to lose the last point of health. May be nil for environment damage" },
		},
		Returns = [[
			If the function returns false or no value, MCServer calls other plugins with this event. If the
			function returns true, no other plugin is called for this event.</p>
			<p>
			In either case, the victim's health is then re-checked and if it is greater than zero, the victim is
			"revived" with that health amount. If the health is less or equal to zero, the victim is killed.
		]],
	},  -- HOOK_KILLING
}




