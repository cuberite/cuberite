return
{
	cArrowEntity =
	{
		Desc = [[
			Represents the arrow when it is shot from the bow. A subclass of the {{cProjectileEntity}}.
		]],
		Functions =
		{
			CanPickup      = { Params = "{{cPlayer|Player}}", Return = "bool", Notes = "Returns true if the specified player can pick the arrow when it's on the ground" },
			GetDamageCoeff = { Params = "", Return = "number", Notes = "Returns the damage coefficient stored within the arrow. The damage dealt by this arrow is multiplied by this coeff" },
			GetPickupState = { Params = "", Return = "PickupState", Notes = "Returns the pickup state (one of the psXXX constants, above)" },
			IsCritical     = { Params = "", Return = "bool", Notes = "Returns true if the arrow should deal critical damage. Based on the bow charge when the arrow was shot." },
			SetDamageCoeff = { Params = "number", Return = "", Notes = "Sets the damage coefficient. The damage dealt by this arrow is multiplied by this coeff" },
			SetIsCritical  = { Params = "bool", Return = "", Notes = "Sets the IsCritical flag on the arrow. Critical arrow deal additional damage" },
			SetPickupState = { Params = "PickupState", Return = "", Notes = "Sets the pickup state (one of the psXXX constants, above)" },
		},
		Constants =
		{
			psInCreative           = { Notes = "The arrow can be picked up only by players in creative gamemode" },
			psInSurvivalOrCreative = { Notes = "The arrow can be picked up by players in survival or creative gamemode" },
			psNoPickup             = { Notes = "The arrow cannot be picked up at all" },
		},
		ConstantGroups =
		{
			PickupState =
			{
				Include = "ps.*",
				TextBefore = [[
					The following constants are used to signalize whether the arrow, once it lands, can be picked by
					players:
				]],
			},
		},
		Inherits = "cProjectileEntity",
	},  -- cArrowEntity

	cFireChargeEntity =
	{
		Desc = "",
		Functions = {},
		Inherits = "cProjectileEntity",
	},  -- cFireChargeEntity

	cGhastFireballEntity =
	{
		Desc = "",
		Functions = {},
		Inherits = "cProjectileEntity",
	},  -- cGhastFireballEntity

	cProjectileEntity =
	{
		Desc = "",
		Functions =
		{
			GetCreator = { Params = "", Return = "{{cEntity}} descendant", Notes = "Returns the entity who created this projectile. May return nil." },
			GetMCAClassName = { Params = "", Return = "string", Notes = "Returns the string that identifies the projectile type  (class name) in MCA files" },
			GetProjectileKind = { Params = "", Return = "ProjectileKind", Notes = "Returns the kind of this projectile (pkXXX constant)" },
			IsInGround = { Params = "", Return = "bool", Notes = "Returns true if this projectile has hit the ground." },
		},
		Constants =
		{
			pkArrow = { Notes = "The projectile is an {{cArrowEntity|arrow}}" },
			pkEgg = { Notes = "The projectile is a {{cThrownEggEntity|thrown egg}}" },
			pkEnderPearl = { Notes = "The projectile is a {{cThrownEnderPearlEntity|thrown enderpearl}}" },
			pkExpBottle = { Notes = "The projectile is a thrown exp bottle (NYI)" },
			pkFireCharge = { Notes = "The projectile is a {{cFireChargeEntity|fire charge}}" },
			pkFirework = { Notes = "The projectile is a (flying) firework (NYI)" },
			pkFishingFloat = { Notes = "The projectile is a fishing float (NYI)" },
			pkGhastFireball = { Notes = "The projectile is a {{cGhastFireballEntity|ghast fireball}}" },
			pkSnowball = { Notes = "The projectile is a {{cThrownSnowballEntity|thrown snowball}}" },
			pkSplashPotion = { Notes = "The projectile is a thrown splash potion (NYI)" },
			pkWitherSkull = { Notes = "The projectile is a wither skull (NYI)" },
		},
		ConstantGroups =
		{
			ProjectileKind =
			{
				Include = "pk.*",
				TextBefore = "The following constants are used to distinguish between the different projectile kinds:",
			},
		},
		Inherits = "cEntity",
	},  -- cProjectileEntity

	cThrownEggEntity =
	{
		Desc = "",
		Functions = {},
		Inherits = "cProjectileEntity",
	},  -- cThrownEggEntity

	cThrownEnderPearlEntity =
	{
		Desc = "",
		Functions = {},
		Inherits = "cProjectileEntity",
	},  -- cThrownEnderPearlEntity

	cThrownSnowballEntity =
	{
		Desc = "",
		Functions = {},
		Inherits = "cProjectileEntity",
	},  -- cThrownSnowballEntity
}




