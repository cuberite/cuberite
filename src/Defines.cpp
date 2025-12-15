#include "Globals.h"

#include "Defines.h"
#include "BlockType.h"





const char * ClickActionToString(int a_ClickAction)
{
	switch (a_ClickAction)
	{
		case caLeftClick:                    return "caLeftClick";
		case caRightClick:                   return "caRightClick";
		case caShiftLeftClick:               return "caShiftLeftClick";
		case caShiftRightClick:              return "caShiftRightClick";
		case caNumber1:                      return "caNumber1";
		case caNumber2:                      return "caNumber2";
		case caNumber3:                      return "caNumber3";
		case caNumber4:                      return "caNumber4";
		case caNumber5:                      return "caNumber5";
		case caNumber6:                      return "caNumber6";
		case caNumber7:                      return "caNumber7";
		case caNumber8:                      return "caNumber8";
		case caNumber9:                      return "caNumber9";
		case caMiddleClick:                  return "caMiddleClick";
		case caDropKey:                      return "caDropKey";
		case caCtrlDropKey:                  return "caCtrlDropKey";
		case caLeftClickOutside:             return "caLeftClickOutside";
		case caRightClickOutside:            return "caRightClickOutside";
		case caLeftClickOutsideHoldNothing:  return "caLeftClickOutsideHoldNothing";
		case caRightClickOutsideHoldNothing: return "caRightClickOutsideHoldNothing";
		case caLeftPaintBegin:               return "caLeftPaintBegin";
		case caRightPaintBegin:              return "caRightPaintBegin";
		case caMiddlePaintBegin:             return "caMiddlePaintBegin";
		case caLeftPaintProgress:            return "caLeftPaintProgress";
		case caRightPaintProgress:           return "caRightPaintProgress";
		case caMiddlePaintProgress:          return "caMiddlePaintProgress";
		case caLeftPaintEnd:                 return "caLeftPaintEnd";
		case caRightPaintEnd:                return "caRightPaintEnd";
		case caMiddlePaintEnd:               return "caMiddlePaintEnd";
		case caDblClick:                     return "caDblClick";
		case caUnknown:                      return "caUnknown";
	}
	UNREACHABLE("Unknown click action");
}





/** Returns a blockface mirrored around the Y axis (doesn't change up / down). */
eBlockFace MirrorBlockFaceY(eBlockFace a_BlockFace)
{
	switch (a_BlockFace)
	{
		case BLOCK_FACE_XM: return BLOCK_FACE_XP;
		case BLOCK_FACE_XP: return BLOCK_FACE_XM;
		case BLOCK_FACE_ZM: return BLOCK_FACE_ZP;
		case BLOCK_FACE_ZP: return BLOCK_FACE_ZM;
		case BLOCK_FACE_NONE:
		case BLOCK_FACE_YM:
		case BLOCK_FACE_YP:
		{
			return a_BlockFace;
		}
	}
	UNREACHABLE("Unsupported block face");
}





eBlockFace MirrorBlockFaceXY(eBlockFace a_BlockFace)
{
	switch (a_BlockFace)
	{
		case BLOCK_FACE_XM: return BLOCK_FACE_XM;
		case BLOCK_FACE_XP: return BLOCK_FACE_XP;
		case BLOCK_FACE_ZM: return BLOCK_FACE_ZP;
		case BLOCK_FACE_ZP: return BLOCK_FACE_ZM;
		case BLOCK_FACE_NONE:
		case BLOCK_FACE_YM:
		case BLOCK_FACE_YP:
		{
			return a_BlockFace;
		}
	}
	UNREACHABLE("Unsupported block face");
}





eBlockFace MirrorBlockFaceYZ(eBlockFace a_BlockFace)
{
	switch (a_BlockFace)
	{
		case BLOCK_FACE_XM: return BLOCK_FACE_XP;
		case BLOCK_FACE_XP: return BLOCK_FACE_XM;
		case BLOCK_FACE_ZM: return BLOCK_FACE_ZM;
		case BLOCK_FACE_ZP: return BLOCK_FACE_ZP;
		case BLOCK_FACE_NONE:
		case BLOCK_FACE_YM:
		case BLOCK_FACE_YP:
		{
			return a_BlockFace;
		}
	}
	UNREACHABLE("Unsupported block face");
}





/** Returns a blockface rotated around the Y axis counter-clockwise. */
eBlockFace RotateBlockFaceCCW(eBlockFace a_BlockFace)
{
	switch (a_BlockFace)
	{
		case BLOCK_FACE_XM: return BLOCK_FACE_ZP;
		case BLOCK_FACE_XP: return BLOCK_FACE_ZM;
		case BLOCK_FACE_ZM: return BLOCK_FACE_XM;
		case BLOCK_FACE_ZP: return BLOCK_FACE_XP;
		case BLOCK_FACE_NONE:
		case BLOCK_FACE_YM:
		case BLOCK_FACE_YP:
		{
			return a_BlockFace;
		}
	}
	UNREACHABLE("Unsupported block face");
}





eBlockFace RotateBlockFaceCW(eBlockFace a_BlockFace)
{
	switch (a_BlockFace)
	{
		case BLOCK_FACE_XM: return BLOCK_FACE_ZM;
		case BLOCK_FACE_XP: return BLOCK_FACE_ZP;
		case BLOCK_FACE_ZM: return BLOCK_FACE_XP;
		case BLOCK_FACE_ZP: return BLOCK_FACE_XM;
		case BLOCK_FACE_NONE:
		case BLOCK_FACE_YM:
		case BLOCK_FACE_YP:
		{
			return a_BlockFace;
		}
	}
	UNREACHABLE("Unsupported block face");
}





eBlockFace ReverseBlockFace(eBlockFace  a_BlockFace)
{
	switch (a_BlockFace)
	{
		case BLOCK_FACE_YP:   return BLOCK_FACE_YM;
		case BLOCK_FACE_XP:   return BLOCK_FACE_XM;
		case BLOCK_FACE_ZP:   return BLOCK_FACE_ZM;
		case BLOCK_FACE_YM:   return BLOCK_FACE_YP;
		case BLOCK_FACE_XM:   return BLOCK_FACE_XP;
		case BLOCK_FACE_ZM:   return BLOCK_FACE_ZP;
		case BLOCK_FACE_NONE: return a_BlockFace;
	}
	UNREACHABLE("Unsupported block face");
}





/** Returns the textual representation of the BlockFace constant. */
AString BlockFaceToString(eBlockFace a_BlockFace)
{
	switch (a_BlockFace)
	{
		case BLOCK_FACE_XM: return "BLOCK_FACE_XM";
		case BLOCK_FACE_XP: return "BLOCK_FACE_XP";
		case BLOCK_FACE_YM: return "BLOCK_FACE_YM";
		case BLOCK_FACE_YP: return "BLOCK_FACE_YP";
		case BLOCK_FACE_ZM: return "BLOCK_FACE_ZM";
		case BLOCK_FACE_ZP: return "BLOCK_FACE_ZP";
		case BLOCK_FACE_NONE: return "BLOCK_FACE_NONE";
	}
	UNREACHABLE("Unsupported block face");
}





/** Here so core plugin can initialize, otherwise useless and deprecated */
bool IsValidItem(Item a_ItemType)
{
	return true;
}





eBlockFace RotationToBlockFace(double a_Rotation, bool a_Inverse)
{
	if (a_Inverse)
	{
		if ((a_Rotation > 135) || (a_Rotation < -135))  // -180/180
		{
			return eBlockFace::BLOCK_FACE_NORTH;
		}
		if ((-45 > a_Rotation) && (a_Rotation >= -135))  // -90
		{
			return eBlockFace::BLOCK_FACE_EAST;
		}
		if ((45 > a_Rotation)  && (a_Rotation >= -45))  // 0
		{
			return eBlockFace::BLOCK_FACE_SOUTH;
		}
		if ((135 > a_Rotation) && (a_Rotation >= 45))  // 90
		{
			return eBlockFace::BLOCK_FACE_WEST;
		}
	}
	else
	{
		if ((a_Rotation > 135) || (a_Rotation < -135))    // -180/180
		{
			return eBlockFace::BLOCK_FACE_SOUTH;
		}
		if ((-45 > a_Rotation) && (a_Rotation >= -135))   // -90
		{
			return eBlockFace::BLOCK_FACE_WEST;
		}
		if ((45 > a_Rotation)  && (a_Rotation >= -45))    // 0
		{
			return eBlockFace::BLOCK_FACE_NORTH;
		}
		if ((135 > a_Rotation) && (a_Rotation >= 45))      // 90
		{
			return eBlockFace::BLOCK_FACE_EAST;
		}
	}
	return BLOCK_FACE_NONE;
}





unsigned char RotationToFineFace(double a_Rotation, bool a_Invert)
{
	unsigned char ret = 0;
	if ((a_Rotation >= - 11.25f) && (a_Rotation < 11.25f))
	{
		// South
		ret = 8;
	}
	else if ((a_Rotation >= 11.25f) && (a_Rotation < 33.75f))
	{
		// SouthSouthWest
		ret = 9;
	}
	else if ((a_Rotation >= 23.75f) && (a_Rotation < 56.25f))
	{
		// SouthWest
		ret = 10;
	}
	else if ((a_Rotation >= 56.25f) && (a_Rotation < 78.75f))
	{
		// WestSouthWest
		ret = 11;
	}
	else if ((a_Rotation >= 78.75f) && (a_Rotation < 101.25f))
	{
		// West
		ret = 12;
	}
	else if ((a_Rotation >= 101.25f) && (a_Rotation < 123.75f))
	{
		// WestNorthWest
		ret = 13;
	}
	else if ((a_Rotation >= 123.75f) && (a_Rotation < 146.25f))
	{
		// NorthWest
		ret = 14;
	}
	else if ((a_Rotation >= 146.25f) && (a_Rotation < 168.75f))
	{
		// NorthNorthWest
		ret = 15;
	}
	else if ((a_Rotation >= -168.75f) && (a_Rotation < -146.25f))
	{
		// NorthNorthEast
		ret = 1;
	}
	else if ((a_Rotation >= -146.25f) && (a_Rotation < -123.75f))
	{
		// NorthEast
		ret = 2;
	}
	else if ((a_Rotation >= -123.75f) && (a_Rotation < -101.25f))
	{
		// EastNorthEast
		ret = 3;
	}
	else if ((a_Rotation >= -101.25) && (a_Rotation < -78.75f))
	{
		// East
		ret = 4;
	}
	else if ((a_Rotation >= -78.75) && (a_Rotation < -56.25f))
	{
		// EastSouthEast
		ret = 5;
	}
	else if ((a_Rotation >= -56.25f) && (a_Rotation < -33.75f))
	{
		// SouthEast
		ret = 6;
	}
	else if ((a_Rotation >= -33.75f) && (a_Rotation < -11.25f))
	{
		// SouthSouthEast
		ret = 7;
	}
	else  // degrees jumping from 180 to -180
	{
		// North
		ret = 0;
	}
	if (a_Invert)
	{
		return (ret + 8) % 16;
	}
	else
	{
		return ret;
	}
}





eBlockFace DisplacementYawToFacing(Vector3d a_PlacePosition, Vector3d a_EyePosition, double a_Yaw)
{
	if (
		const auto Displacement = a_EyePosition - a_PlacePosition.addedXZ(0.5, 0.5);
			(std::abs(Displacement.x) < 2) && (std::abs(Displacement.z) < 2)
			)
	{
		if (Displacement.y > 2)
		{
			return BLOCK_FACE_TOP;
		}

		if (Displacement.y < 0)
		{
			return BLOCK_FACE_BOTTOM;
		}
	}

	return RotationToBlockFace(a_Yaw);
}





eDimension StringToDimension(const AString & a_DimensionString)
{
	// First try decoding as a number
	int res;
	if (StringToInteger(a_DimensionString, res))
	{
		// It was a valid number
		return static_cast<eDimension>(res);
	}

	// Decode using a built-in map:
	static struct
	{
		eDimension m_Dimension;
		const char * m_String;
	} DimensionMap [] =
	{
		{ dimOverworld, "Overworld"},
		{ dimOverworld, "Normal"},
		{ dimOverworld, "World"},
		{ dimNether,    "Nether"},
		{ dimNether,    "Hell"},  // Alternate name for Nether
		{ dimEnd,       "End"},
		{ dimEnd,       "Sky"},  // Old name for End
	} ;
	for (size_t i = 0; i < ARRAYCOUNT(DimensionMap); i++)
	{
		if (NoCaseCompare(DimensionMap[i].m_String, a_DimensionString) == 0)
		{
			return DimensionMap[i].m_Dimension;
		}
	}  // for i - DimensionMap[]

	// Not found
	LOGWARNING("Unknown dimension: \"%s\". Setting to Overworld", a_DimensionString.c_str());
	return dimOverworld;
}





AString DimensionToString(eDimension a_Dimension)
{
	// Decode using a built-in map:
	static struct
	{
		eDimension m_Dimension;
		const char * m_String;
	} DimensionMap[] =
	{
		{ dimOverworld, "Overworld" },
		{ dimNether, "Nether" },
		{ dimEnd, "End" },
	};

	for (size_t i = 0; i < ARRAYCOUNT(DimensionMap); i++)
	{
		if (DimensionMap[i].m_Dimension == a_Dimension)
		{
			return DimensionMap[i].m_String;
		}
	}  // for i - DimensionMap[]

	// Not found
	LOGWARNING("Unknown dimension: \"%i\". Setting to Overworld", static_cast<int>(a_Dimension));
	return "Overworld";
}





AString DamageTypeToString(eDamageType a_DamageType)
{
	// Make sure to keep this alpha-sorted.
	switch (a_DamageType)
	{
		case dtAdmin:           return "dtAdmin";
		case dtAttack:          return "dtAttack";
		case dtCactusContact:   return "dtCactusContact";
		case dtMagmaContact:    return "dtMagmaContact";
		case dtDrowning:        return "dtDrowning";
		case dtEnderPearl:      return "dtEnderPearl";
		case dtEnvironment:     return "dtEnvironment";
		case dtFalling:         return "dtFalling";
		case dtFireContact:     return "dtFireContact";
		case dtInVoid:          return "dtInVoid";
		case dtLavaContact:     return "dtLavaContact";
		case dtLightning:       return "dtLightning";
		case dtOnFire:          return "dtOnFire";
		case dtPoisoning:       return "dtPoisoning";
		case dtWithering:       return "dtWithering";
		case dtPotionOfHarming: return "dtPotionOfHarming";
		case dtRangedAttack:    return "dtRangedAttack";
		case dtStarving:        return "dtStarving";
		case dtSuffocating:     return "dtSuffocation";
		case dtExplosion:       return "dtExplosion";
	}
	UNREACHABLE("Unsupported damage type");
}





eDamageType StringToDamageType(const AString & a_DamageTypeString)
{
	// First try decoding as a number:
	int res;
	if (!StringToInteger(a_DamageTypeString, res))
	{
		// It was a valid number
		return static_cast<eDamageType>(res);
	}

	// Decode using a built-in map:
	static struct
	{
		eDamageType  m_DamageType;
		const char * m_String;
	} DamageTypeMap [] =
	{
		// Cannonical names:
		{ dtAttack,          "dtAttack"},
		{ dtRangedAttack,    "dtRangedAttack"},
		{ dtLightning,       "dtLightning"},
		{ dtFalling,         "dtFalling"},
		{ dtDrowning,        "dtDrowning"},
		{ dtSuffocating,     "dtSuffocation"},
		{ dtStarving,        "dtStarving"},
		{ dtCactusContact,   "dtCactusContact"},
		{ dtMagmaContact,    "dtMagmaContact"},
		{ dtLavaContact,     "dtLavaContact"},
		{ dtPoisoning,       "dtPoisoning"},
		{ dtWithering,       "dtWithering"},
		{ dtOnFire,          "dtOnFire"},
		{ dtFireContact,     "dtFireContact"},
		{ dtInVoid,          "dtInVoid"},
		{ dtPotionOfHarming, "dtPotionOfHarming"},
		{ dtAdmin,           "dtAdmin"},
		{ dtExplosion,       "dtExplosion"},
		{ dtEnvironment,     "dtEnvironment"},

		// Common synonyms:
		{ dtAttack,        "dtPawnAttack"},
		{ dtAttack,        "dtEntityAttack"},
		{ dtAttack,        "dtMob"},
		{ dtAttack,        "dtMobAttack"},
		{ dtRangedAttack,  "dtArrowAttack"},
		{ dtRangedAttack,  "dtArrow"},
		{ dtRangedAttack,  "dtProjectile"},
		{ dtFalling,       "dtFall"},
		{ dtDrowning,      "dtDrown"},
		{ dtSuffocating,   "dtSuffocation"},
		{ dtStarving,      "dtStarvation"},
		{ dtStarving,      "dtHunger"},
		{ dtCactusContact, "dtCactus"},
		{ dtCactusContact, "dtCactuses"},
		{ dtCactusContact, "dtCacti"},
		{ dtMagmaContact,  "dtMagma"},
		{ dtLavaContact,   "dtLava"},
		{ dtPoisoning,     "dtPoison"},
		{ dtWithering,     "dtWither"},
		{ dtOnFire,        "dtBurning"},
		{ dtFireContact,   "dtInFire"},
		{ dtAdmin,         "dtPlugin"},
	} ;
	for (size_t i = 0; i < ARRAYCOUNT(DamageTypeMap); i++)
	{
		if (NoCaseCompare(DamageTypeMap[i].m_String, a_DamageTypeString) == 0)
		{
			return DamageTypeMap[i].m_DamageType;
		}
	}  // for i - DamageTypeMap[]

	// Not found:
	return static_cast<eDamageType>(-1);
}





void AddFaceDirection(int & a_BlockX, int & a_BlockY, int & a_BlockZ, eBlockFace a_BlockFace, bool a_bInverse)
{
	LOGWARNING("AddFaceDirection with X/Y/Z parameters is deprecated, use the vector version");

	const auto Offset = AddFaceDirection({ a_BlockX, a_BlockY, a_BlockZ }, a_BlockFace, a_bInverse);
	a_BlockX = Offset.x;
	a_BlockY = Offset.y;
	a_BlockZ = Offset.z;
}





Vector3i AddFaceDirection(const Vector3i a_Position, const eBlockFace a_BlockFace, const bool a_InvertDirection)
{
	const int Offset = a_InvertDirection ? -1 : 1;

	switch (a_BlockFace)
	{
		case BLOCK_FACE_YP: return a_Position.addedY(+Offset);
		case BLOCK_FACE_YM: return a_Position.addedY(-Offset);
		case BLOCK_FACE_ZM: return a_Position.addedZ(-Offset);
		case BLOCK_FACE_ZP: return a_Position.addedZ(+Offset);
		case BLOCK_FACE_XP: return a_Position.addedX(+Offset);
		case BLOCK_FACE_XM: return a_Position.addedX(-Offset);
		case BLOCK_FACE_NONE:
		{
			LOGWARNING("%s: Unknown face: %s", __FUNCTION__, BlockFaceToString(a_BlockFace));
			ASSERT(!"AddFaceDirection(): Unknown face");
			break;
		}
	}

	UNREACHABLE("Unsupported block face");
}





bool ItemCategory::IsPickaxe(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::WoodenPickaxe:
		case Item::StonePickaxe:
		case Item::IronPickaxe:
		case Item::GoldenPickaxe:
		case Item::DiamondPickaxe:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool ItemCategory::IsAxe(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::WoodenAxe:
		case Item::StoneAxe:
		case Item::IronAxe:
		case Item::GoldenAxe:
		case Item::DiamondAxe:
			return true;
		default: return false;
	}
}





bool ItemCategory::IsSword(Item a_ItemID)
{
	switch (a_ItemID)
	{
		case Item::WoodenSword:
		case Item::StoneSword:
		case Item::IronSword:
		case Item::GoldenSword:
		case Item::DiamondSword:
			return true;
		default: return false;
	}
}





bool ItemCategory::IsHoe(Item a_ItemID)
{
	switch (a_ItemID)
	{
		case Item::WoodenHoe:
		case Item::StoneHoe:
		case Item::IronHoe:
		case Item::GoldenHoe:
		case Item::DiamondHoe:
			return true;
		default: return false;
	}
}





bool ItemCategory::IsShovel(Item a_ItemID)
{
	switch (a_ItemID)
	{
		case Item::WoodenShovel:
		case Item::StoneShovel:
		case Item::IronShovel:
		case Item::GoldenShovel:
		case Item::DiamondShovel:
			return true;
		default: return false;
	}
}





bool ItemCategory::IsTool(Item a_ItemID)
{
	return IsPickaxe( a_ItemID)
		|| IsAxe    ( a_ItemID)
		|| IsSword  ( a_ItemID)
		|| IsHoe    ( a_ItemID)
		|| IsShovel ( a_ItemID);
}





bool ItemCategory::IsHelmet(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::LeatherHelmet:
		case Item::GoldenHelmet:
		case Item::ChainmailHelmet:
		case Item::IronHelmet:
		case Item::DiamondHelmet:
			return true;
		default: return false;
	}
}





bool ItemCategory::IsChestPlate(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::Elytra:
		case Item::LeatherChestplate:
		case Item::GoldenChestplate:
		case Item::ChainmailChestplate:
		case Item::IronChestplate:
		case Item::DiamondChestplate:
			return true;
		default: return false;
	}
}





bool ItemCategory::IsLeggings(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::LeatherLeggings:
		case Item::GoldenLeggings:
		case Item::ChainmailLeggings:
		case Item::IronLeggings:
		case Item::DiamondLeggings:
			return true;
		default: return false;
	}
}





bool ItemCategory::IsBoots(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::LeatherBoots:
		case Item::GoldenBoots:
		case Item::ChainmailBoots:
		case Item::IronBoots:
		case Item::DiamondBoots:
			return true;
		default: return false;
	}
}





bool ItemCategory::IsMinecart(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::Minecart:
		case Item::ChestMinecart:
		case Item::FurnaceMinecart:
		case Item::TNTMinecart:
		case Item::HopperMinecart:
			return true;
		default: return false;
	}
}





bool ItemCategory::IsArmor(Item a_ItemType)
{
	return (
		IsHelmet(a_ItemType) ||
		IsChestPlate(a_ItemType) ||
		IsLeggings(a_ItemType) ||
		IsBoots(a_ItemType)
	);
}





bool ItemCategory::IsHorseArmor(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::IronHorseArmor:
		case Item::GoldenHorseArmor:
		case Item::DiamondHorseArmor:
			return true;
		default: return false;
	}
}





bool ItemCategory::IsVillagerFood(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::Carrot:
		case Item::Potato:
		case Item::Bread:
		case Item::Beetroot:
		case Item::WheatSeeds:
		case Item::BeetrootSeeds:
		case Item::Wheat:
		{
			return true;
		}

		default:
		{
			return false;
		}
	}
}
