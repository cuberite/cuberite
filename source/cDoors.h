#pragma once
#include "cBlockToPickup.h"


class cDoors												//tolua_export
{															//tolua_export
public:
	static char RotationToMetaData( float a_Rotation )		//tolua_export
	{														//tolua_export
		a_Rotation += 90 + 45; // So its not aligned with axis
		if( a_Rotation > 360.f ) a_Rotation -= 360.f;
		if( a_Rotation >= 0.f && a_Rotation < 90.f )
			return 0x0;
		else if( a_Rotation >= 180 && a_Rotation < 270 )
			return 0x2;
		else if( a_Rotation >= 90 && a_Rotation < 180 )
			return 0x1;
		else
			return 0x3;
	}														//tolua_export

	static char ChangeStateMetaData( char a_MetaData )		//tolua_export
	{														//tolua_export

		a_MetaData ^= 4;		//XOR bit 2 aka 3. bit (Door open state)

		return a_MetaData;
	}														//tolua_export

	static void ChangeDoor(cWorld *a_World, int a_X, int a_Y, int a_Z)	//tolua_export
	{																	//tolua_export
		char OldMetaData = a_World->GetBlockMeta(a_X, a_Y, a_Z);
		
		a_World->SetBlockMeta(a_X, a_Y, a_Z, ChangeStateMetaData ( OldMetaData ) );

		
		if (OldMetaData & 8)
		{ //current block is top of the door
			char BottomBlock = a_World->GetBlock(a_X, a_Y - 1, a_Z);
			char BottomMeta = a_World->GetBlockMeta(a_X, a_Y - 1, a_Z);

			if (IsDoor(BottomBlock) && !(BottomMeta & 8))
			{
				a_World->SetBlockMeta(a_X, a_Y - 1, a_Z, ChangeStateMetaData ( BottomMeta ) );
			}
		} else { //current block is bottom of the door
			char TopBlock = a_World->GetBlock(a_X, a_Y + 1, a_Z);
			char TopMeta = a_World->GetBlockMeta(a_X, a_Y + 1, a_Z);

			if (IsDoor(TopBlock) && (TopMeta & 8))
			{
				a_World->SetBlockMeta(a_X, a_Y + 1, a_Z, ChangeStateMetaData ( TopMeta ) );
			}
		}
	}																	//tolua_export
	
	inline static bool IsDoor(char a_Block)
	{
		return (a_Block == E_BLOCK_WOODEN_DOOR || a_Block == E_BLOCK_IRON_DOOR);
	}

};															//tolua_export