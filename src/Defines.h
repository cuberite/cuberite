
#pragma once





/** List of slot numbers, used for inventory-painting */
typedef std::vector<int> cSlotNums;






// tolua_begin

/** Experience Orb setup */
enum
{
	// Open to suggestion on naming convention here :)
	MAX_EXPERIENCE_ORB_SIZE = 2000
} ;





/** Block face constants, used in PlayerDigging and PlayerBlockPlacement packets and bbox collision calc */
enum eBlockFace
{
	BLOCK_FACE_NONE = -1,  // Interacting with no block face - swinging the item in the air
	BLOCK_FACE_XM   =  4,  // Interacting with the X- face of the block
	BLOCK_FACE_XP   =  5,  // Interacting with the X+ face of the block
	BLOCK_FACE_YM   =  0,  // Interacting with the Y- face of the block
	BLOCK_FACE_YP   =  1,  // Interacting with the Y+ face of the block
	BLOCK_FACE_ZM   =  2,  // Interacting with the Z- face of the block
	BLOCK_FACE_ZP   =  3,  // Interacting with the Z+ face of the block

	// Synonyms using the (deprecated) world directions:
	BLOCK_FACE_BOTTOM = BLOCK_FACE_YM,  // Interacting with the bottom   face of the block
	BLOCK_FACE_TOP    = BLOCK_FACE_YP,  // Interacting with the top      face of the block
	BLOCK_FACE_NORTH  = BLOCK_FACE_ZM,  // Interacting with the northern face of the block
	BLOCK_FACE_SOUTH  = BLOCK_FACE_ZP,  // Interacting with the southern face of the block
	BLOCK_FACE_WEST   = BLOCK_FACE_XM,  // Interacting with the western  face of the block
	BLOCK_FACE_EAST   = BLOCK_FACE_XP,  // Interacting with the eastern  face of the block

	// Bounds, used for range-checking:
	BLOCK_FACE_MIN = -1,
	BLOCK_FACE_MAX =  5,
} ;





/** PlayerDigging status constants */
enum
{
	DIG_STATUS_STARTED           = 0,
	DIG_STATUS_CANCELLED         = 1,
	DIG_STATUS_FINISHED          = 2,
	DIG_STATUS_DROP_STACK        = 3,
	DIG_STATUS_DROP_HELD         = 4,
	DIG_STATUS_SHOOT_EAT         = 5,
	DIG_STATUS_SWAP_ITEM_IN_HAND = 6,
} ;





/** Individual actions sent in the WindowClick packet */
enum eClickAction
{
	// Sorted by occurrence in the 1.5 protocol
	caLeftClick,
	caRightClick,
	caShiftLeftClick,
	caShiftRightClick,
	caNumber1,
	caNumber2,
	caNumber3,
	caNumber4,
	caNumber5,
	caNumber6,
	caNumber7,
	caNumber8,
	caNumber9,
	caMiddleClick,
	caDropKey,
	caCtrlDropKey,
	caLeftClickOutside,
	caRightClickOutside,
	caLeftClickOutsideHoldNothing,
	caRightClickOutsideHoldNothing,
	caLeftPaintBegin,
	caRightPaintBegin,
	caMiddlePaintBegin,
	caLeftPaintProgress,
	caRightPaintProgress,
	caMiddlePaintProgress,
	caLeftPaintEnd,
	caRightPaintEnd,
	caMiddlePaintEnd,
	caDblClick,
	// Add new actions here
	caUnknown = 255,

	// Keep this list in sync with ClickActionToString() function below!
} ;





enum eGameMode
{
	eGameMode_NotSet    = -1,
	eGameMode_Survival  = 0,
	eGameMode_Creative  = 1,
	eGameMode_Adventure = 2,
	eGameMode_Spectator = 3,

	// Easier-to-use synonyms:
	gmNotSet    = eGameMode_NotSet,
	gmSurvival  = eGameMode_Survival,
	gmCreative  = eGameMode_Creative,
	gmAdventure = eGameMode_Adventure,
	gmSpectator = eGameMode_Spectator,

	// These two are used to check GameMode for validity when converting from integers.
	gmMax,  // Gets automatically assigned
	gmMin = 0,
} ;





enum eChatType
{
	ctChatBox        = 0,
	ctSystem         = 1,
	ctAboveActionBar = 2,
} ;





enum eWeather
{
	eWeather_Sunny        = 0,
	eWeather_Rain         = 1,
	eWeather_ThunderStorm = 2,

	// Easier-to-use synonyms:
	wSunny        = eWeather_Sunny,
	wRain         = eWeather_Rain,
	wThunderstorm = eWeather_ThunderStorm,
	wStorm        = wThunderstorm,
} ;





enum eMobHeadType
{
	SKULL_TYPE_SKELETON    = 0,
	SKULL_TYPE_WITHER      = 1,
	SKULL_TYPE_ZOMBIE      = 2,
	SKULL_TYPE_PLAYER      = 3,
	SKULL_TYPE_CREEPER     = 4,
	SKULL_TYPE_DRAGON      = 5,
} ;





enum eMobHeadRotation
{
	SKULL_ROTATION_NORTH = 0,
	SKULL_ROTATION_NORTH_NORTH_EAST = 1,
	SKULL_ROTATION_NORTH_EAST = 2,
	SKULL_ROTATION_EAST_NORTH_EAST = 3,
	SKULL_ROTATION_EAST = 4,
	SKULL_ROTATION_EAST_SOUTH_EAST = 5,
	SKULL_ROTATION_SOUTH_EAST = 6,
	SKULL_ROTATION_SOUTH_SOUTH_EAST = 7,
	SKULL_ROTATION_SOUTH = 8,
	SKULL_ROTATION_SOUTH_SOUTH_WEST = 9,
	SKULL_ROTATION_SOUTH_WEST = 10,
	SKULL_ROTATION_WEST_SOUTH_WEST = 11,
	SKULL_ROTATION_WEST = 12,
	SKULL_ROTATION_WEST_NORTH_WEST = 13,
	SKULL_ROTATION_NORTH_WEST = 14,
	SKULL_ROTATION_NORTH_NORTH_WEST = 15,
} ;





enum eHand
{
	hMain = 0,
	hOff = 1,
} ;





enum eMainHand
{
	mhLeft = 0,
	mhRight = 1,
} ;





enum eSkinPart
{
	spCape = 0x01,
	spJacket = 0x02,
	spLeftSleeve = 0x04,
	spRightSleeve = 0x08,
	spLeftPants = 0x10,
	spRightPants = 0x20,
	spHat = 0x40,
	spMask = 0x7F,
};





inline const char * ClickActionToString(int a_ClickAction)
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
inline eBlockFace MirrorBlockFaceY(eBlockFace a_BlockFace)
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
inline eBlockFace RotateBlockFaceCCW(eBlockFace a_BlockFace)
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





inline eBlockFace RotateBlockFaceCW(eBlockFace a_BlockFace)
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





inline eBlockFace ReverseBlockFace(eBlockFace  a_BlockFace)
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
inline AString BlockFaceToString(eBlockFace a_BlockFace)
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





inline bool IsValidBlock(int a_BlockType)
{
	if (
		(
		(a_BlockType > -1) &&
		(a_BlockType <= E_BLOCK_MAX_TYPE_ID)
		) ||
		(a_BlockType == 255)  // the blocks 253-254 don't exist yet -> https://minecraft.gamepedia.com/Data_values#Block_IDs
	)
	{
		return true;
	}
	return false;
}





inline bool IsValidItem(int a_ItemType)
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

// tolua_end





inline bool IsBlockWater(BLOCKTYPE a_BlockType)
{
	return ((a_BlockType == E_BLOCK_WATER) || (a_BlockType == E_BLOCK_STATIONARY_WATER));
}





inline bool IsBlockIce(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_ICE:
		case E_BLOCK_PACKED_ICE:
		case E_BLOCK_FROSTED_ICE:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





inline bool IsBlockWaterOrIce(BLOCKTYPE a_BlockType)
{
	return (IsBlockWater(a_BlockType) || IsBlockIce(a_BlockType));
}





inline bool IsBlockLava(BLOCKTYPE a_BlockType)
{
	return ((a_BlockType == E_BLOCK_LAVA) || (a_BlockType == E_BLOCK_STATIONARY_LAVA));
}





inline bool IsBlockLiquid(BLOCKTYPE a_BlockType)
{
	return IsBlockWater(a_BlockType) || IsBlockLava(a_BlockType);
}




inline bool IsBlockRail(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_RAIL:
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_POWERED_RAIL:
		{
			return true;
		}
		default: return false;
	}
}





inline bool IsBlockTypeOfDirt(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_DIRT:
		case E_BLOCK_GRASS:
		case E_BLOCK_FARMLAND:
		case E_BLOCK_GRASS_PATH:
		{
			return true;
		}
	}
	return false;
}




inline bool IsBlockFence(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_ACACIA_FENCE:
		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_BIRCH_FENCE:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_COBBLESTONE_WALL:
		case E_BLOCK_DARK_OAK_FENCE:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_FENCE:
		case E_BLOCK_JUNGLE_FENCE:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_NETHER_BRICK_FENCE:
		case E_BLOCK_OAK_FENCE_GATE:
		case E_BLOCK_SPRUCE_FENCE:
		case E_BLOCK_SPRUCE_FENCE_GATE:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





inline bool IsBlockMaterialWood(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_PLANKS:
		case E_BLOCK_LOG:
		case E_BLOCK_NOTE_BLOCK:
		case E_BLOCK_BOOKCASE:
		case E_BLOCK_OAK_WOOD_STAIRS:
		case E_BLOCK_CHEST:
		case E_BLOCK_CRAFTING_TABLE:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_OAK_DOOR:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		case E_BLOCK_JUKEBOX:
		case E_BLOCK_FENCE:
		case E_BLOCK_TRAPDOOR:
		case E_BLOCK_HUGE_BROWN_MUSHROOM:
		case E_BLOCK_HUGE_RED_MUSHROOM:
		case E_BLOCK_OAK_FENCE_GATE:
		case E_BLOCK_DOUBLE_WOODEN_SLAB:
		case E_BLOCK_WOODEN_SLAB:
		case E_BLOCK_SPRUCE_WOOD_STAIRS:
		case E_BLOCK_BIRCH_WOOD_STAIRS:
		case E_BLOCK_JUNGLE_WOOD_STAIRS:
		case E_BLOCK_TRAPPED_CHEST:
		case E_BLOCK_DAYLIGHT_SENSOR:
		case E_BLOCK_NEW_LOG:
		case E_BLOCK_ACACIA_WOOD_STAIRS:
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_WALL_BANNER:
		case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:
		case E_BLOCK_SPRUCE_FENCE_GATE:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_SPRUCE_FENCE:
		case E_BLOCK_BIRCH_FENCE:
		case E_BLOCK_JUNGLE_FENCE:
		case E_BLOCK_DARK_OAK_FENCE:
		case E_BLOCK_ACACIA_FENCE:
		case E_BLOCK_SPRUCE_DOOR:
		case E_BLOCK_BIRCH_DOOR:
		case E_BLOCK_JUNGLE_DOOR:
		case E_BLOCK_ACACIA_DOOR:
		case E_BLOCK_DARK_OAK_DOOR:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





inline bool IsBlockMaterialPlants(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_SAPLING:
		case E_BLOCK_DANDELION:
		case E_BLOCK_FLOWER:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_CROPS:
		case E_BLOCK_REEDS:
		case E_BLOCK_PUMPKIN_STEM:
		case E_BLOCK_MELON_STEM:
		case E_BLOCK_LILY_PAD:
		case E_BLOCK_NETHER_WART:
		case E_BLOCK_COCOA_POD:
		case E_BLOCK_CARROTS:
		case E_BLOCK_POTATOES:
		case E_BLOCK_CHORUS_PLANT:
		case E_BLOCK_CHORUS_FLOWER:
		case E_BLOCK_BEETROOTS:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





inline bool IsBlockMaterialVine(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_VINES:
		case E_BLOCK_BIG_FLOWER:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





inline bool IsBlockMaterialIron(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_LAPIS_BLOCK:
		case E_BLOCK_GOLD_BLOCK:
		case E_BLOCK_IRON_BLOCK:
		case E_BLOCK_DIAMOND_BLOCK:
		case E_BLOCK_IRON_DOOR:
		case E_BLOCK_IRON_BARS:
		case E_BLOCK_BREWING_STAND:
		case E_BLOCK_CAULDRON:
		case E_BLOCK_EMERALD_BLOCK:
		case E_BLOCK_COMMAND_BLOCK:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_BLOCK_OF_REDSTONE:
		case E_BLOCK_HOPPER:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_REPEATING_COMMAND_BLOCK:
		case E_BLOCK_CHAIN_COMMAND_BLOCK:
		case E_BLOCK_STRUCTURE_BLOCK:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





inline bool IsBlockMaterialAnvil(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_ANVIL);
}





inline bool IsBlocksWeb(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_COBWEB);
}





inline bool IsBlockMaterialLeaves(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_LEAVES) || (a_BlockType == E_BLOCK_NEW_LEAVES);
}





inline bool IsBlocksWool(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_WOOL);
}





inline bool IsBlockMaterialGourd(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_PUMPKIN:
		case E_BLOCK_JACK_O_LANTERN:
		case E_BLOCK_MELON:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





inline bool IsBlockMaterialCoral(BLOCKTYPE a_BlockType)
{
	return false;  // yes, there is no block in minecraft which belongs to this type.
}





inline bool IsBlockMaterialRock(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_STONE:
		case E_BLOCK_COBBLESTONE:
		case E_BLOCK_BEDROCK:
		case E_BLOCK_GOLD_ORE:
		case E_BLOCK_IRON_ORE:
		case E_BLOCK_COAL_ORE:
		case E_BLOCK_LAPIS_ORE:
		case E_BLOCK_DISPENSER:
		case E_BLOCK_SANDSTONE:
		case E_BLOCK_DOUBLE_STONE_SLAB:
		case E_BLOCK_STONE_SLAB:
		case E_BLOCK_BRICK:
		case E_BLOCK_MOSSY_COBBLESTONE:
		case E_BLOCK_OBSIDIAN:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_DIAMOND_ORE:
		case E_BLOCK_FURNACE:
		case E_BLOCK_LIT_FURNACE:
		case E_BLOCK_COBBLESTONE_STAIRS:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_REDSTONE_ORE:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
		case E_BLOCK_NETHERRACK:
		case E_BLOCK_STONE_BRICKS:
		case E_BLOCK_BRICK_STAIRS:
		case E_BLOCK_STONE_BRICK_STAIRS:
		case E_BLOCK_NETHER_BRICK:
		case E_BLOCK_NETHER_BRICK_FENCE:
		case E_BLOCK_NETHER_BRICK_STAIRS:
		case E_BLOCK_ENCHANTMENT_TABLE:
		case E_BLOCK_END_PORTAL_FRAME:
		case E_BLOCK_END_STONE:
		case E_BLOCK_SANDSTONE_STAIRS:
		case E_BLOCK_EMERALD_ORE:
		case E_BLOCK_ENDER_CHEST:
		case E_BLOCK_COBBLESTONE_WALL:
		case E_BLOCK_NETHER_QUARTZ_ORE:
		case E_BLOCK_QUARTZ_BLOCK:
		case E_BLOCK_QUARTZ_STAIRS:
		case E_BLOCK_DROPPER:
		case E_BLOCK_STAINED_CLAY:
		case E_BLOCK_PRISMARINE_BLOCK:
		case E_BLOCK_HARDENED_CLAY:
		case E_BLOCK_BLOCK_OF_COAL:
		case E_BLOCK_RED_SANDSTONE:
		case E_BLOCK_RED_SANDSTONE_STAIRS:
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:
		case E_BLOCK_RED_SANDSTONE_SLAB:
		case E_BLOCK_PURPUR_BLOCK:
		case E_BLOCK_PURPUR_PILLAR:
		case E_BLOCK_PURPUR_STAIRS:
		case E_BLOCK_PURPUR_DOUBLE_SLAB:
		case E_BLOCK_PURPUR_SLAB:
		case E_BLOCK_END_BRICKS:
		case E_BLOCK_MAGMA:
		case E_BLOCK_RED_NETHER_BRICK:
		case E_BLOCK_BONE_BLOCK:
		case E_BLOCK_OBSERVER:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





inline void AddFaceDirection(int & a_BlockX, int & a_BlockY, int & a_BlockZ, eBlockFace a_BlockFace, bool a_bInverse = false)  // tolua_export
{  // tolua_export
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
}  // tolua_export





inline void AddFaceDirection(int & a_BlockX, unsigned char & a_BlockY, int & a_BlockZ, eBlockFace a_BlockFace, bool a_bInverse = false)
{
	int Y = a_BlockY;
	AddFaceDirection(a_BlockX, Y, a_BlockZ, a_BlockFace, a_bInverse);
	a_BlockY = Clamp<unsigned char>(static_cast<unsigned char>(Y), 0, cChunkDef::Height - 1);
}





inline void EulerToVector(double a_Pan, double a_Pitch, double & a_X, double & a_Y, double & a_Z)
{
	// 	a_X = sinf ( a_Pan / 180 * PI) * cosf ( a_Pitch / 180 * PI);
	// 	a_Y = -sinf ( a_Pitch / 180 * PI);
	// 	a_Z = -cosf ( a_Pan / 180 * PI) * cosf ( a_Pitch / 180 * PI);
	a_X = cos(a_Pan / 180 * M_PI) * cos(a_Pitch / 180 * M_PI);
	a_Y = sin(a_Pan / 180 * M_PI) * cos(a_Pitch / 180 * M_PI);
	a_Z = sin(a_Pitch / 180 * M_PI);
}





inline void VectorToEuler(double a_X, double a_Y, double a_Z, double & a_Pan, double & a_Pitch)
{
	double r = sqrt((a_X * a_X) + (a_Z * a_Z));
	if (r < std::numeric_limits<double>::epsilon())
	{
		a_Pan = 0;
	}
	else
	{
		a_Pan = atan2(a_Z, a_X) * 180 / M_PI - 90;
	}

	a_Pitch = atan2(a_Y, r) * 180 / M_PI;
}





inline float GetSignf(float a_Val)
{
	return (a_Val < 0.f) ? -1.f : 1.f;
}





inline float GetSpecialSignf( float a_Val)
{
	return (a_Val <= 0.f) ? -1.f : 1.f;
}





template <class T> inline T Diff(T a_Val1, T a_Val2)
{
	return std::abs(a_Val1 - a_Val2);
}





// tolua_begin

enum eMessageType
{
	// https://forum.cuberite.org/thread-1212.html
	// MessageType...

	mtCustom,          // Send raw data without any processing
	mtFailure,         // Something could not be done (i.e. command not executed due to insufficient privilege)
	mtInformation,     // Informational message (i.e. command usage)
	mtSuccess,         // Something executed successfully
	mtWarning,         // Something concerning (i.e. reload) is about to happen
	mtFatal,           // Something catastrophic occured (i.e. plugin crash)
	mtDeath,           // Denotes death of player
	mtPrivateMessage,  // Player to player messaging identifier
	mtJoin,            // A player has joined the server
	mtLeave,           // A player has left the server
	mtMaxPlusOne,      // The first invalid type, used for checking on LuaAPI boundaries

	// Common aliases:
	mtFail  = mtFailure,
	mtError = mtFailure,
	mtInfo  = mtInformation,
	mtPM    = mtPrivateMessage,
};





/** Normalizes an angle in degrees to the [-180, +180) range: */
inline double NormalizeAngleDegrees(const double a_Degrees)
{
	double Norm = fmod(a_Degrees + 180, 360);
	if (Norm < 0)
	{
		Norm += 360;
	}
	return Norm - 180;
}





namespace ItemCategory
{
	inline bool IsPickaxe(short a_ItemID)
	{
		return (a_ItemID == E_ITEM_WOODEN_PICKAXE)
			|| (a_ItemID == E_ITEM_STONE_PICKAXE)
			|| (a_ItemID == E_ITEM_IRON_PICKAXE)
			|| (a_ItemID == E_ITEM_GOLD_PICKAXE)
			|| (a_ItemID == E_ITEM_DIAMOND_PICKAXE);
	}



	inline bool IsAxe(short a_ItemID)
	{
		return (a_ItemID == E_ITEM_WOODEN_AXE)
			|| (a_ItemID == E_ITEM_STONE_AXE)
			|| (a_ItemID == E_ITEM_IRON_AXE)
			|| (a_ItemID == E_ITEM_GOLD_AXE)
			|| (a_ItemID == E_ITEM_DIAMOND_AXE);
	}



	inline bool IsSword(short a_ItemID)
	{
		return (a_ItemID == E_ITEM_WOODEN_SWORD)
			|| (a_ItemID == E_ITEM_STONE_SWORD)
			|| (a_ItemID == E_ITEM_IRON_SWORD)
			|| (a_ItemID == E_ITEM_GOLD_SWORD)
			|| (a_ItemID == E_ITEM_DIAMOND_SWORD);
	}



	inline bool IsHoe(short a_ItemID)
	{
		return (a_ItemID == E_ITEM_WOODEN_HOE)
			|| (a_ItemID == E_ITEM_STONE_HOE)
			|| (a_ItemID == E_ITEM_IRON_HOE)
			|| (a_ItemID == E_ITEM_GOLD_HOE)
			|| (a_ItemID == E_ITEM_DIAMOND_HOE);
	}



	inline bool IsShovel(short a_ItemID)
	{
		return (a_ItemID == E_ITEM_WOODEN_SHOVEL)
			|| (a_ItemID == E_ITEM_STONE_SHOVEL)
			|| (a_ItemID == E_ITEM_IRON_SHOVEL)
			|| (a_ItemID == E_ITEM_GOLD_SHOVEL)
			|| (a_ItemID == E_ITEM_DIAMOND_SHOVEL);
	}



	inline bool IsTool(short a_ItemID)
	{
		return IsPickaxe( a_ItemID)
			|| IsAxe    ( a_ItemID)
			|| IsSword  ( a_ItemID)
			|| IsHoe    ( a_ItemID)
			|| IsShovel ( a_ItemID);
	}



	inline bool IsHelmet(short a_ItemType)
	{
		return (
			(a_ItemType == E_ITEM_LEATHER_CAP) ||
			(a_ItemType == E_ITEM_GOLD_HELMET) ||
			(a_ItemType == E_ITEM_CHAIN_HELMET) ||
			(a_ItemType == E_ITEM_IRON_HELMET) ||
			(a_ItemType == E_ITEM_DIAMOND_HELMET)
		);
	}



	inline bool IsChestPlate(short a_ItemType)
	{
		return (
			(a_ItemType == E_ITEM_LEATHER_TUNIC) ||
			(a_ItemType == E_ITEM_GOLD_CHESTPLATE) ||
			(a_ItemType == E_ITEM_CHAIN_CHESTPLATE) ||
			(a_ItemType == E_ITEM_IRON_CHESTPLATE) ||
			(a_ItemType == E_ITEM_DIAMOND_CHESTPLATE)
		);
	}



	inline bool IsLeggings(short a_ItemType)
	{
		return (
			(a_ItemType == E_ITEM_LEATHER_PANTS) ||
			(a_ItemType == E_ITEM_GOLD_LEGGINGS) ||
			(a_ItemType == E_ITEM_CHAIN_LEGGINGS) ||
			(a_ItemType == E_ITEM_IRON_LEGGINGS) ||
			(a_ItemType == E_ITEM_DIAMOND_LEGGINGS)
		);
	}



	inline bool IsBoots(short a_ItemType)
	{
		return (
			(a_ItemType == E_ITEM_LEATHER_BOOTS) ||
			(a_ItemType == E_ITEM_GOLD_BOOTS) ||
			(a_ItemType == E_ITEM_CHAIN_BOOTS) ||
			(a_ItemType == E_ITEM_IRON_BOOTS) ||
			(a_ItemType == E_ITEM_DIAMOND_BOOTS)
		);
	}



	inline bool IsMinecart(short a_ItemType)
	{
		return (
			(a_ItemType == E_ITEM_MINECART) ||
			(a_ItemType == E_ITEM_CHEST_MINECART) ||
			(a_ItemType == E_ITEM_FURNACE_MINECART) ||
			(a_ItemType == E_ITEM_MINECART_WITH_TNT) ||
			(a_ItemType == E_ITEM_MINECART_WITH_HOPPER)
		);
	}



	inline bool IsArmor(short a_ItemType)
	{
		return (
			IsHelmet(a_ItemType) ||
			IsChestPlate(a_ItemType) ||
			IsLeggings(a_ItemType) ||
			IsBoots(a_ItemType)
		);
	}



	inline bool IsHorseArmor(short a_ItemType)
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
}

// tolua_end
