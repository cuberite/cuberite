
#pragma once





typedef unsigned char Byte;

/// List of slot numbers, used for inventory-painting
typedef std::vector<int> cSlotNums;






// tolua_begin

/// How much light do the blocks emit on their own?
extern unsigned char g_BlockLightValue[];

/// How much light do the block consume?
extern unsigned char g_BlockSpreadLightFalloff[];

/// Is a block completely transparent? (light doesn't get decreased(?))
extern bool g_BlockTransparent[];

/// Is a block destroyed after a single hit?
extern bool g_BlockOneHitDig[];

/// Can a piston break this block?
extern bool g_BlockPistonBreakable[256];

/// Can this block hold snow atop?
extern bool g_BlockIsSnowable[256];

/// Does this block require a tool to drop?
extern bool g_BlockRequiresSpecialTool[256];

/// Is this block solid (player cannot walk through)?
extern bool g_BlockIsSolid[256];

/// Can torches be placed on this block?
extern bool g_BlockIsTorchPlaceable[256];

/// Experience Orb setup
enum
{
	//open to suggestion on naming convention here :)
	MAX_EXPERIENCE_ORB_SIZE = 2000
} ;





/// Block face constants, used in PlayerDigging and PlayerBlockPlacement packets and bbox collision calc
enum eBlockFace
{
	BLOCK_FACE_NONE = -1,  // Interacting with no block face - swinging the item in the air
	BLOCK_FACE_XM   = 4,   // Interacting with the X- face of the block
	BLOCK_FACE_XP   = 5,   // Interacting with the X+ face of the block
	BLOCK_FACE_YM   = 0,   // Interacting with the Y- face of the block
	BLOCK_FACE_YP   = 1,   // Interacting with the Y+ face of the block
	BLOCK_FACE_ZM   = 2,   // Interacting with the Z- face of the block
	BLOCK_FACE_ZP   = 3,   // Interacting with the Z+ face of the block
	
	// Synonyms using the (deprecated) world directions:
	BLOCK_FACE_BOTTOM = BLOCK_FACE_YM,  // Interacting with the bottom   face of the block
	BLOCK_FACE_TOP    = BLOCK_FACE_YP,  // Interacting with the top      face of the block
	BLOCK_FACE_NORTH  = BLOCK_FACE_ZM,  // Interacting with the northern face of the block
	BLOCK_FACE_SOUTH  = BLOCK_FACE_ZP,  // Interacting with the southern face of the block
	BLOCK_FACE_WEST   = BLOCK_FACE_XM,  // Interacting with the western  face of the block
	BLOCK_FACE_EAST   = BLOCK_FACE_XP,  // Interacting with the eastern  face of the block
} ;





/// PlayerDigging status constants
enum
{
	DIG_STATUS_STARTED   = 0,
	DIG_STATUS_CANCELLED = 1,
	DIG_STATUS_FINISHED  = 2,
	DIG_STATUS_DROP_HELD = 4,
	DIG_STATUS_SHOOT_EAT = 5,
} ;





/// Individual actions sent in the WindowClick packet
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
	caLeftPaintProgress,
	caRightPaintProgress,
	caLeftPaintEnd,
	caRightPaintEnd,
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
	
	// Easier-to-use synonyms:
	gmNotSet    = eGameMode_NotSet,
	gmSurvival  = eGameMode_Survival,
	gmCreative  = eGameMode_Creative,
	gmAdventure = eGameMode_Adventure,
	
	// These two are used to check GameMode for validity when converting from integers.
	gmMax,  // Gets automatically assigned
	gmMin = 0,
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





inline const char * ClickActionToString(eClickAction a_ClickAction)
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
		case caLeftPaintProgress:            return "caLeftPaintProgress";
		case caRightPaintProgress:           return "caRightPaintProgress";
		case caLeftPaintEnd:                 return "caLeftPaintEnd";
		case caRightPaintEnd:                return "caRightPaintEnd";
		case caDblClick:                     return "caDblClick";
		
		case caUnknown:                      return "caUnknown";
	}
	ASSERT(!"Unknown click action");
	return "caUnknown";
}





inline bool IsValidBlock(int a_BlockType)
{
	if (
		(a_BlockType > -1) &&
		(a_BlockType <= E_BLOCK_MAX_TYPE_ID)
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





inline bool IsBlockLava(BLOCKTYPE a_BlockType)
{
	return ((a_BlockType == E_BLOCK_LAVA) || (a_BlockType == E_BLOCK_STATIONARY_LAVA));
}





inline bool IsBlockLiquid(BLOCKTYPE a_BlockType)
{
	return IsBlockWater(a_BlockType) || IsBlockLava(a_BlockType);
}





inline bool IsBlockTypeOfDirt(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_DIRT:
		case E_BLOCK_GRASS:
		case E_BLOCK_FARMLAND:
		{
			return true;
		}
	}
	return false;
}




inline void AddFaceDirection(int & a_BlockX, int & a_BlockY, int & a_BlockZ, char a_BlockFace, bool a_bInverse = false)  // tolua_export
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
			default:
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
			default:
			{
				LOGWARNING("%s: Unknown inv face: %d", __FUNCTION__, a_BlockFace);
				ASSERT(!"AddFaceDirection(): Unknown face");
				break;
			}
		}
	}
}  // tolua_export





inline void AddFaceDirection(int & a_BlockX, unsigned char & a_BlockY, int & a_BlockZ, char a_BlockFace, bool a_bInverse = false)
{
	int Y = a_BlockY;
	AddFaceDirection(a_BlockX, Y, a_BlockZ, a_BlockFace, a_bInverse);
	if (Y < 0)
	{
		a_BlockY = 0;
	}
	else if (Y > 255)
	{
		a_BlockY = 255;
	}
	else
	{
		a_BlockY = (unsigned char)Y;
	}
}





#define PI 3.14159265358979323846264338327950288419716939937510582097494459072381640628620899862803482534211706798f

inline void EulerToVector(double a_Pan, double a_Pitch, double & a_X, double & a_Y, double & a_Z)
{
	// 	a_X = sinf ( a_Pan / 180 * PI ) * cosf ( a_Pitch / 180 * PI );
	// 	a_Y = -sinf ( a_Pitch / 180 * PI );
	// 	a_Z = -cosf ( a_Pan / 180 * PI ) * cosf ( a_Pitch / 180 * PI );
	a_X = cos(a_Pan / 180 * PI) * cos(a_Pitch / 180 * PI);
	a_Y = sin(a_Pan / 180 * PI) * cos(a_Pitch / 180 * PI);
	a_Z = sin(a_Pitch / 180 * PI);
}





inline void VectorToEuler(double a_X, double a_Y, double a_Z, double & a_Pan, double & a_Pitch)
{
	if (a_X != 0)
	{
		a_Pan = atan2(a_Z, a_X) * 180 / PI - 90;
	}
	else
	{
		a_Pan = 0;
	}
	a_Pitch = atan2(a_Y, sqrt((a_X * a_X) + (a_Z * a_Z))) * 180 / PI;
}





inline float GetSignf(float a_Val)
{
	return (a_Val < 0.f) ? -1.f : 1.f;
}





inline float GetSpecialSignf( float a_Val )
{
	return (a_Val <= 0.f) ? -1.f : 1.f;
}




// tolua_begin

/// Normalizes an angle in degrees to the [-180, +180) range:
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
		return IsPickaxe( a_ItemID )
			|| IsAxe    ( a_ItemID )
			|| IsSword  ( a_ItemID )
			|| IsHoe    ( a_ItemID )
			|| IsShovel ( a_ItemID );
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
	
	
	
	inline bool IsArmor(short a_ItemType)
	{
		return (
			IsHelmet(a_ItemType) ||
			IsChestPlate(a_ItemType) ||
			IsLeggings(a_ItemType) ||
			IsBoots(a_ItemType)
		);
	}
}

// tolua_end


inline bool BlockRequiresSpecialTool(BLOCKTYPE a_BlockType)
{
	if(!IsValidBlock(a_BlockType)) return false;
	return g_BlockRequiresSpecialTool[a_BlockType];
}






