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





bool IsValidBlock(int a_BlockType)
{
	return (
		((a_BlockType > -1) && (a_BlockType <= E_BLOCK_MAX_TYPE_ID)) ||
		(a_BlockType == 255)  // the blocks 253-254 don't exist yet -> https://minecraft.gamepedia.com/Data_values#Block_IDs
	);
}





bool IsValidItem(int a_ItemType)
{
	if (
		((a_ItemType >= E_ITEM_FIRST) && (a_ItemType <= E_ITEM_MAX_CONSECUTIVE_TYPE_ID)) ||  // Basic items range
		((a_ItemType >= E_ITEM_FIRST_DISC) && (a_ItemType <= E_ITEM_LAST_DISC))   // Music discs' special range
	)
	{
		return true;
	}

	if (a_ItemType == 0)
	{
		return false;
	}

	return IsValidBlock(a_ItemType);
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
	if (!a_bInverse)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YP: a_BlockY++; break;
			case BLOCK_FACE_YM: a_BlockY--; break;
			case BLOCK_FACE_ZM: a_BlockZ--; break;
			case BLOCK_FACE_ZP: a_BlockZ++; break;
			case BLOCK_FACE_XP: a_BlockX++; break;
			case BLOCK_FACE_XM: a_BlockX--; break;
			case BLOCK_FACE_NONE:
			{
				LOGWARNING("%s: Unknown face: %d", __FUNCTION__, a_BlockFace);
				ASSERT(!"AddFaceDirection(): Unknown face");
				break;
			}
		}
	}
	else
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YP: a_BlockY--; break;
			case BLOCK_FACE_YM: a_BlockY++; break;
			case BLOCK_FACE_ZM: a_BlockZ++; break;
			case BLOCK_FACE_ZP: a_BlockZ--; break;
			case BLOCK_FACE_XP: a_BlockX--; break;
			case BLOCK_FACE_XM: a_BlockX++; break;
			case BLOCK_FACE_NONE:
			{
				LOGWARNING("%s: Unknown inv face: %d", __FUNCTION__, a_BlockFace);
				ASSERT(!"AddFaceDirection(): Unknown face");
				break;
			}
		}
	}
}





bool ItemCategory::IsPickaxe(short a_ItemType)
{
	switch (a_ItemType)
	{
		case E_ITEM_WOODEN_PICKAXE:
		case E_ITEM_STONE_PICKAXE:
		case E_ITEM_IRON_PICKAXE:
		case E_ITEM_GOLD_PICKAXE:
		case E_ITEM_DIAMOND_PICKAXE:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool ItemCategory::IsAxe(short a_ItemType)
{
	switch (a_ItemType)
	{
		case E_ITEM_WOODEN_AXE:
		case E_ITEM_STONE_AXE:
		case E_ITEM_IRON_AXE:
		case E_ITEM_GOLD_AXE:
		case E_ITEM_DIAMOND_AXE:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool ItemCategory::IsSword(short a_ItemID)
{
	return (a_ItemID == E_ITEM_WOODEN_SWORD)
		|| (a_ItemID == E_ITEM_STONE_SWORD)
		|| (a_ItemID == E_ITEM_IRON_SWORD)
		|| (a_ItemID == E_ITEM_GOLD_SWORD)
		|| (a_ItemID == E_ITEM_DIAMOND_SWORD);
}





bool ItemCategory::IsHoe(short a_ItemID)
{
	return (a_ItemID == E_ITEM_WOODEN_HOE)
		|| (a_ItemID == E_ITEM_STONE_HOE)
		|| (a_ItemID == E_ITEM_IRON_HOE)
		|| (a_ItemID == E_ITEM_GOLD_HOE)
		|| (a_ItemID == E_ITEM_DIAMOND_HOE);
}





bool ItemCategory::IsShovel(short a_ItemID)
{
	return (a_ItemID == E_ITEM_WOODEN_SHOVEL)
		|| (a_ItemID == E_ITEM_STONE_SHOVEL)
		|| (a_ItemID == E_ITEM_IRON_SHOVEL)
		|| (a_ItemID == E_ITEM_GOLD_SHOVEL)
		|| (a_ItemID == E_ITEM_DIAMOND_SHOVEL);
}





bool ItemCategory::IsTool(short a_ItemID)
{
	return IsPickaxe( a_ItemID)
		|| IsAxe    ( a_ItemID)
		|| IsSword  ( a_ItemID)
		|| IsHoe    ( a_ItemID)
		|| IsShovel ( a_ItemID);
}





bool ItemCategory::IsHelmet(short a_ItemType)
{
	return (
		(a_ItemType == E_ITEM_LEATHER_CAP) ||
		(a_ItemType == E_ITEM_GOLD_HELMET) ||
		(a_ItemType == E_ITEM_CHAIN_HELMET) ||
		(a_ItemType == E_ITEM_IRON_HELMET) ||
		(a_ItemType == E_ITEM_DIAMOND_HELMET)
	);
}





bool ItemCategory::IsChestPlate(short a_ItemType)
{
	return (
		(a_ItemType == E_ITEM_LEATHER_TUNIC) ||
		(a_ItemType == E_ITEM_GOLD_CHESTPLATE) ||
		(a_ItemType == E_ITEM_CHAIN_CHESTPLATE) ||
		(a_ItemType == E_ITEM_IRON_CHESTPLATE) ||
		(a_ItemType == E_ITEM_DIAMOND_CHESTPLATE)
	);
}





bool ItemCategory::IsLeggings(short a_ItemType)
{
	return (
		(a_ItemType == E_ITEM_LEATHER_PANTS) ||
		(a_ItemType == E_ITEM_GOLD_LEGGINGS) ||
		(a_ItemType == E_ITEM_CHAIN_LEGGINGS) ||
		(a_ItemType == E_ITEM_IRON_LEGGINGS) ||
		(a_ItemType == E_ITEM_DIAMOND_LEGGINGS)
	);
}





bool ItemCategory::IsBoots(short a_ItemType)
{
	return (
		(a_ItemType == E_ITEM_LEATHER_BOOTS) ||
		(a_ItemType == E_ITEM_GOLD_BOOTS) ||
		(a_ItemType == E_ITEM_CHAIN_BOOTS) ||
		(a_ItemType == E_ITEM_IRON_BOOTS) ||
		(a_ItemType == E_ITEM_DIAMOND_BOOTS)
	);
}





bool ItemCategory::IsMinecart(short a_ItemType)
{
	return (
		(a_ItemType == E_ITEM_MINECART) ||
		(a_ItemType == E_ITEM_CHEST_MINECART) ||
		(a_ItemType == E_ITEM_FURNACE_MINECART) ||
		(a_ItemType == E_ITEM_MINECART_WITH_TNT) ||
		(a_ItemType == E_ITEM_MINECART_WITH_HOPPER)
	);
}





bool ItemCategory::IsArmor(short a_ItemType)
{
	return (
		IsHelmet(a_ItemType) ||
		IsChestPlate(a_ItemType) ||
		IsLeggings(a_ItemType) ||
		IsBoots(a_ItemType)
	);
}





bool ItemCategory::IsHorseArmor(short a_ItemType)
{
	switch (a_ItemType)
	{
		case E_ITEM_IRON_HORSE_ARMOR:
		case E_ITEM_GOLD_HORSE_ARMOR:
		case E_ITEM_DIAMOND_HORSE_ARMOR:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}
