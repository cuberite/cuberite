
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"
#include "WorldInterface.h"
#include "MetaRotator.h"
#include "../Entities/Player.h"





class cBlockBedHandler :
	public cMetaRotator<cBlockHandler, 0x3, 0x02, 0x03, 0x00, 0x01, true>
{
public:
	cBlockBedHandler(BLOCKTYPE a_BlockType)
		: cMetaRotator<cBlockHandler, 0x3, 0x02, 0x03, 0x00, 0x01, true>(a_BlockType)
	{
	}


	virtual void OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;


	virtual bool IsUseable(void) override
	{
		return true;
	}

	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(E_ITEM_BED, 1, 0));
	}


	virtual bool CanDirtGrowGrass(NIBBLETYPE a_Meta) override
	{
		return true;
	}



	// Bed specific helper functions
	static NIBBLETYPE RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 180 + (180 / 4);  // So its not aligned with axis
		if (a_Rotation > 360) a_Rotation -= 360;

		a_Rotation = (a_Rotation / 360) * 4;

		return ((char)a_Rotation + 2) % 4;
	}


	static Vector3i MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0: return Vector3i(0, 0, 1);
			case 1: return Vector3i(-1, 0, 0);
			case 2: return Vector3i(0, 0, -1);
			case 3: return Vector3i(1, 0, 0);
		}
		return Vector3i();
	}
} ;




