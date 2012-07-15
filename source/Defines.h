#pragma once

#include "BlockID.h"

//tolua_begin
// emissive blocks
extern unsigned char g_BlockLightValue[];
// whether blocks allow spreading
extern unsigned char g_BlockSpreadLightFalloff[];
// whether blocks are transparent (light can shine though)
extern bool g_BlockTransparent[];
// one hit break blocks
extern bool g_BlockOneHitDig[];
//tolua_end

//--DO NOT DELETE THIS COMMENT-- //tolua_export





inline bool IsValidBlock( int a_BlockID )	//tolua_export
{											//tolua_export
	if( a_BlockID > -1 &&
		a_BlockID <= 126 && //items to 109 are valid for Beta1.8.1.. 1.2.5 is up to 126
		//a_BlockID != 29 && allow pistons
		//a_BlockID != 33 && allow pistons
		a_BlockID != 34 &&
		a_BlockID != 36 )
	{
		return true;
	}
	return false;
}											//tolua_export





// Was old :o
// Changed to fit the style ;)
inline bool IsValidItem( int a_ItemID )		//tolua_export
{											//tolua_export
	if( (a_ItemID >= 256 && a_ItemID <= 388)
		|| (a_ItemID >= 2256 && a_ItemID <= 2266) )
	{
		return true;
	}

	if( a_ItemID == 0 )
		return false;

	return IsValidBlock( a_ItemID );
}											//tolua_export





inline bool IsBlockWater(char a_BlockID)
{
	return (a_BlockID == E_BLOCK_WATER || a_BlockID == E_BLOCK_STATIONARY_WATER);
}





inline bool IsBlockLava(char a_BlockID)
{
	return (a_BlockID == E_BLOCK_LAVA || a_BlockID == E_BLOCK_STATIONARY_LAVA);
}


inline bool IsBlockLiquid(char a_BlockID)
{
	return IsBlockWater(a_BlockID) || IsBlockLava(a_BlockID);
}

inline bool IsBlockTypeOfDirt(char a_BlockID)
{
	switch (a_BlockID)
	{
		case E_BLOCK_DIRT:
		case E_BLOCK_GRASS:
		case E_BLOCK_FARMLAND:
			return true;
	}
	return false;
}




inline void AddDirection( int & a_X, int & a_Y, int & a_Z, char a_Direction, bool a_bInverse = false )
{
	if( !a_bInverse )
	{
		switch( a_Direction )
		{
		case 0:
			a_Y--;
			break;
		case 1:
			a_Y++;
			break;
		case 2:
			a_Z--;
			break;
		case 3:
			a_Z++;
			break;
		case 4:
			a_X--;
			break;
		case 5:
			a_X++;
			break;
		};
	}
	else
	{
		switch( a_Direction )	// other way around
		{
		case 0:
			a_Y++;
			break;
		case 1:
			a_Y--;
			break;
		case 2:
			a_Z++;
			break;
		case 3:
			a_Z--;
			break;
		case 4:
			a_X++;
			break;
		case 5:
			a_X--;
			break;
		};
	}
}





inline void AddDirection( int & a_X, unsigned char & a_Y, int & a_Z, char a_Direction, bool a_bInverse = false ) //tolua_export
{//tolua_export
	int Y = a_Y;
	AddDirection( a_X, Y, a_Z, a_Direction, a_bInverse );
	if( Y < 0 ) a_Y = 0;
	else if( Y > 255 ) a_Y = 255;
	else a_Y = (unsigned char)Y;
}//tolua_export





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




//tolua_begin
namespace ItemCategory
{
	inline bool IsPickaxe(ENUM_ITEM_ID a_ItemID)
	{
		return (a_ItemID == E_ITEM_WOODEN_PICKAXE)
			|| (a_ItemID == E_ITEM_STONE_PICKAXE)
			|| (a_ItemID == E_ITEM_IRON_PICKAXE)
			|| (a_ItemID == E_ITEM_GOLD_PICKAXE)
			|| (a_ItemID == E_ITEM_DIAMOND_PICKAXE);
	}



	inline bool IsAxe(ENUM_ITEM_ID a_ItemID)
	{
		return (a_ItemID == E_ITEM_WOODEN_AXE)
			|| (a_ItemID == E_ITEM_STONE_AXE)
			|| (a_ItemID == E_ITEM_IRON_AXE)
			|| (a_ItemID == E_ITEM_GOLD_AXE)
			|| (a_ItemID == E_ITEM_DIAMOND_AXE);
	}



	inline bool IsSword(ENUM_ITEM_ID a_ItemID)
	{
		return (a_ItemID == E_ITEM_WOODEN_SWORD)
			|| (a_ItemID == E_ITEM_STONE_SWORD)
			|| (a_ItemID == E_ITEM_IRON_SWORD)
			|| (a_ItemID == E_ITEM_GOLD_SWORD)
			|| (a_ItemID == E_ITEM_DIAMOND_SWORD);
	}



	inline bool IsHoe(ENUM_ITEM_ID a_ItemID)
	{
		return (a_ItemID == E_ITEM_WOODEN_HOE)
			|| (a_ItemID == E_ITEM_STONE_HOE)
			|| (a_ItemID == E_ITEM_IRON_HOE)
			|| (a_ItemID == E_ITEM_GOLD_HOE)
			|| (a_ItemID == E_ITEM_DIAMOND_HOE);
	}
	
	
	
	inline bool IsShovel(ENUM_ITEM_ID a_ItemID)
	{
		return (a_ItemID == E_ITEM_WOODEN_SHOVEL)
			|| (a_ItemID == E_ITEM_STONE_SHOVEL)
			|| (a_ItemID == E_ITEM_IRON_SHOVEL)
			|| (a_ItemID == E_ITEM_GOLD_SHOVEL)
			|| (a_ItemID == E_ITEM_DIAMOND_SHOVEL);
	}



	inline bool IsTool(ENUM_ITEM_ID a_ItemID)
	{
		return IsPickaxe( a_ItemID )
			|| IsAxe    ( a_ItemID )
			|| IsSword  ( a_ItemID )
			|| IsHoe    ( a_ItemID )
			|| IsShovel ( a_ItemID );
	}
}
//tolua_end


inline bool BlockRequiresSpecialTool(BLOCKTYPE a_BlockID)
{
	if(!IsValidBlock(a_BlockID)) return false;
	return g_BlockRequiresSpecialTool[a_BlockID];
}


//tolua_begin
enum eGameMode
{
	eGameMode_NotSet	  = -1,
	eGameMode_Survival    = 0,
	eGameMode_Creative    = 1,
};





enum eWeather
{
	eWeather_Sunny		  = 0,
	eWeather_Rain		  = 1,
	eWeather_ThunderStorm = 2,

};




//tolua_end




