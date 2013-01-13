
#pragma once

#include "BlockID.h"

typedef unsigned char Byte;

// tolua_begin

/// How much light do the blocks emit on their own?
extern unsigned char g_BlockLightValue[];

/// How much light do the block consume?
extern unsigned char g_BlockSpreadLightFalloff[];

/// Is a block completely transparent? (light doesn't get decreased(?))
extern bool g_BlockTransparent[];

/// Is a block destroyed after a single hit?
extern bool g_BlockOneHitDig[];





// Block face constants, used in PlayerDigging and PlayerBlockPlacement packets
enum
{
	BLOCK_FACE_NONE   = -1,  // Interacting with no block face - swinging the item in the air
	BLOCK_FACE_BOTTOM = 0,   // Interacting with the bottom   face of the block
	BLOCK_FACE_TOP    = 1,   // Interacting with the top      face of the block
	BLOCK_FACE_NORTH  = 2,   // Interacting with the northern face of the block
	BLOCK_FACE_SOUTH  = 3,   // Interacting with the southern face of the block
	BLOCK_FACE_WEST   = 4,   // Interacting with the western  face of the block
	BLOCK_FACE_EAST   = 5,   // Interacting with the eastern  face of the block
} ;

// PlayerDigging status constants:
enum
{
	DIG_STATUS_STARTED   = 0,
	DIG_STATUS_CANCELLED = 1,
	DIG_STATUS_FINISHED  = 2,
	DIG_STATUS_DROP_HELD = 4,
	DIG_STATUS_SHOOT_EAT = 5,
} ;
// tolua_end





inline bool IsValidBlock(int a_BlockType)	// tolua_export
{											// tolua_export
	if (
		(a_BlockType > -1) &&
		(a_BlockType <= E_BLOCK_MAX_TYPE_ID) &&
		(a_BlockType != 34) &&  // Piston extension
		(a_BlockType != 36)     // Piston moved block
	)
	{
		return true;
	}
	return false;
}											// tolua_export





inline bool IsValidItem(int a_ItemType)		// tolua_export
{											// tolua_export
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
}											// tolua_export





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
			case BLOCK_FACE_BOTTOM: a_BlockY--; break;
			case BLOCK_FACE_TOP:    a_BlockY++; break;
			case BLOCK_FACE_EAST:   a_BlockX++; break;
			case BLOCK_FACE_WEST:   a_BlockX--; break;
			case BLOCK_FACE_NORTH:  a_BlockZ--; break;
			case BLOCK_FACE_SOUTH:  a_BlockZ++; break;
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
			case BLOCK_FACE_BOTTOM: a_BlockY++; break;
			case BLOCK_FACE_TOP:    a_BlockY--; break;
			case BLOCK_FACE_EAST:   a_BlockX--; break;
			case BLOCK_FACE_WEST:   a_BlockX++; break;
			case BLOCK_FACE_NORTH:  a_BlockZ++; break;
			case BLOCK_FACE_SOUTH:  a_BlockZ--; break;
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





#include <math.h>
#define PI				3.14159265358979323846264338327950288419716939937510582097494459072381640628620899862803482534211706798f
#define MIN(a,b) (((a)>(b))?(b):(a))
#define MAX(a,b) (((a)>(b))?(a):(b))
inline void EulerToVector( float a_Pan, float a_Pitch, float & a_X, float & a_Y, float & a_Z )
{
	// 	a_X = sinf ( a_Pan / 180 * PI ) * cosf ( a_Pitch / 180 * PI );
	// 	a_Y = -sinf ( a_Pitch / 180 * PI );
	// 	a_Z = -cosf ( a_Pan / 180 * PI ) * cosf ( a_Pitch / 180 * PI );
	a_X = cos(a_Pan / 180 * PI)*cos(a_Pitch / 180 * PI);
	a_Y = sin(a_Pan / 180 * PI)*cos(a_Pitch / 180 * PI);
	a_Z = sin(a_Pitch / 180 * PI);
}





inline void VectorToEuler( float a_X, float a_Y, float a_Z, float & a_Pan, float & a_Pitch )
{
	if( a_X != 0 )
		a_Pan = atan2( a_Z, a_X ) * 180 / PI - 90;
	else
		a_Pan = 0;
	a_Pitch = atan2(a_Y, sqrtf((a_X * a_X) + (a_Z * a_Z))) * 180 / PI;
}





inline float GetSignf( float a_Val )
{
	return (a_Val < 0.f)?-1.f:1.f;
}





inline float GetSpecialSignf( float a_Val )
{
	return (a_Val <= 0.f)?-1.f:1.f;
}




// tolua_begin
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
}
// tolua_end


inline bool BlockRequiresSpecialTool(BLOCKTYPE a_BlockType)
{
	if(!IsValidBlock(a_BlockType)) return false;
	return g_BlockRequiresSpecialTool[a_BlockType];
}


// tolua_begin
enum eGameMode
{
	eGameMode_NotSet    = -1,
	eGameMode_Survival  = 0,
	eGameMode_Creative  = 1,
	eGameMode_Adventure = 2
};





enum eWeather
{
	eWeather_Sunny        = 0,
	eWeather_Rain         = 1,
	eWeather_ThunderStorm = 2
};




// tolua_end




