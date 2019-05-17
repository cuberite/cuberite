return
{
	cArmorStand =
	{
		Inherits =
		{
			"cEntity",
		},
		Functions =
		{
			GetBodyRotation =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "Vector3<double>",
						},
					},
				},
			},
			GetCustomName =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "const AString",
						},
					},
					Desc = "Gets the custom name of the armor stand. If no custom name is set, the function returns an empty string.",
				},
			},
			GetEquippedBoots =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
				},
			},
			GetEquippedChestplate =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
				},
			},
			GetEquippedHelmet =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
				},
			},
			GetEquippedLeggings =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
				},
			},
			GetEquippedWeapon =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
				},
			},
			GetHeadRotation =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "Vector3<double>",
						},
					},
				},
			},
			GetLeftArmRotation =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "Vector3<double>",
						},
					},
				},
			},
			GetLeftLegRotation =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "Vector3<double>",
						},
					},
				},
			},
			GetOffHandEquipedItem =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
				},
			},
			GetRightArmRotation =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "Vector3<double>",
						},
					},
				},
			},
			GetRightLegRotation =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "Vector3<double>",
						},
					},
				},
			},
			HasArms =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "bool",
						},
					},
					Desc = "Returns true if the armor stand have amrs.",
				},
			},
			HasBasePlate =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "bool",
						},
					},
					Desc = "Returns true if the armor stand have a base platform.",
				},
			},
			HasCustomName =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "bool",
						},
					},
					Desc = "Returns true if the armor stand has a custom name.",
				},
			},
			HasGravity =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "bool",
						},
					},
					Desc = "Returns true if the armor stand has gravity.",
				},
			},
			IsCustomNameAlwaysVisible =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "bool",
						},
					},
					Desc = "Is the custom name of this armor stand always visible? If not, you only see the name when you sight the mob.",
				},
			},
			IsInvisible =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "bool",
						},
					},
					Desc = "Returns true if the armor stand is invisible.",
				},
			},
			IsMarker =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "bool",
						},
					},
					Desc = "Returns true if the armor stand is a marker (null size).",
				},
			},
			IsNormal =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "bool",
						},
					},
					Desc = "Returns true if the armor stand is a normal sized one.",
				},
			},
			IsSmall =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "bool",
						},
					},
					Desc = "Returns true if the armor stand is a small one.",
				},
			},
			SetBodyRotation =
			{
				{
					Params =
					{
						{
							Type = "Vector3<double>",
							Name = "a_BodyRotation",
						},
					},
					Returns =
					{
					},
				},
			},
			SetCustomName =
			{
				{
					Params =
					{
						{
							Type = "AString",
							Name = "a_CustomName",
						},
					},
					Returns =
					{
						{
							Type = "AString",
						},
					},
					Desc = "Sets the custom name of the armor stand. You see the name over the armor stand.\
\9If you want to disable the custom name, simply set an empty string.",
				},
			},
			SetCustomNameAlwaysVisible =
			{
				{
					Params =
					{
						{
							Type = "bool",
							Name = "a_CustomNameAlwaysVisible",
						},
					},
					Returns =
					{
					},
					Desc = "Sets the custom name visiblity of this armor stand.\
\9If it's false, you only see the name when you sight the mob. If it's true, you always see the custom name.",
				},
			},
			SetEquippedBoots =
			{
				{
					Params =
					{
						{
							Type = "cItem",
							Name = "a_Boots",
						},
					},
					Returns =
					{
					},
				},
			},
			SetEquippedChestplate =
			{
				{
					Params =
					{
						{
							Type = "cItem",
							Name = "a_ChestPlate",
						},
					},
					Returns =
					{
					},
				},
			},
			SetEquippedHelmet =
			{
				{
					Params =
					{
						{
							Type = "cItem",
							Name = "a_Helmet",
						},
					},
					Returns =
					{
					},
				},
			},
			SetEquippedLeggings =
			{
				{
					Params =
					{
						{
							Type = "cItem",
							Name = "a_Leggings",
						},
					},
					Returns =
					{
					},
				},
			},
			SetEquippedWeapon =
			{
				{
					Params =
					{
						{
							Type = "cItem",
							Name = "a_RightHand",
						},
					},
					Returns =
					{
					},
				},
			},
			SetHasArms =
			{
				{
					Params =
					{
						{
							Type = "bool",
							Name = "a_HasArms",
						},
					},
					Returns =
					{
					},
				},
			},
			SetHasBasePlate =
			{
				{
					Params =
					{
						{
							Type = "bool",
							Name = "a_HasBasePlate",
						},
					},
					Returns =
					{
					},
				},
			},
			SetHeadRotation =
			{
				{
					Params =
					{
						{
							Type = "Vector3<double>",
							Name = "a_HeadRotation",
						},
					},
					Returns =
					{
					},
				},
			},
			SetIsMarker =
			{
				{
					Params =
					{
						{
							Type = "bool",
							Name = "a_IsMarker",
						},
					},
					Returns =
					{
					},
					Desc = "Sets wathever the armor stand is a marker (null size by default).",
				},
			},
			SetLeftArmRotation =
			{
				{
					Params =
					{
						{
							Type = "Vector3<double>",
							Name = "a_LeftArmRotation",
						},
					},
					Returns =
					{
					},
				},
			},
			SetLeftLegRotation =
			{
				{
					Params =
					{
						{
							Type = "Vector3<double>",
							Name = "a_LeftLegRotation",
						},
					},
					Returns =
					{
					},
				},
			},
			SetNormal =
			{
				{
					Params =
					{
					},
					Returns =
					{
					},
					Desc = "Sets if the armor stand is a normal sized one.",
				},
			},
			SetOffHandEquipedItem =
			{
				{
					Params =
					{
						{
							Type = "cItem",
							Name = "a_LeftHand",
						},
					},
					Returns =
					{
					},
				},
			},
			SetRightArmRotation =
			{
				{
					Params =
					{
						{
							Type = "Vector3<double>",
							Name = "a_RightArmRotation",
						},
					},
					Returns =
					{
					},
				},
			},
			SetRightLegRotation =
			{
				{
					Params =
					{
						{
							Type = "Vector3<double>",
							Name = "a_RightLegRotation",
						},
					},
					Returns =
					{
					},
				},
			},
			SetSmall =
			{
				{
					Params =
					{
					},
					Returns =
					{
					},
					Desc = "Sets if the armor stand is a small one.",
				},
			},
			SetVisible =
			{
				{
					Params =
					{
						{
							Type = "bool",
							Name = "a_IsVisible",
						},
					},
					Returns =
					{
					},
					Desc = "Set whatever the armorstand is visible",
				},
			},
		},
	},
}
