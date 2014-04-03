return
{
	HOOK_PROJECTILE_HIT_BLOCK =
	{
		CalledWhen = "A projectile hits a solid block.",
		DefaultFnName = "OnProjectileHitBlock",  -- also used as pagename
		Desc = [[
			This hook is called when a {{cProjectileEntity|projectile}} hits a solid block..
		]],
		Params =
		{
			{ Name = "ProjectileEntity", Type = "{{cProjectileEntity}}", Notes = "The projectile that hit an entity." },
		},
		Returns = [[
			If the function returns false or no value, the next plugin's callback is called. If the function
			returns true, no other callback is called for this event and the projectile flies through block..
		]],
	},  -- HOOK_PROJECTILE_HIT_BLOCK
}





