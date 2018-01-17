
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemNetherWartHandler :
	public cItemHandler
{
public:
	cItemNetherWartHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}

	virtual bool IsPlaceable(void) override
	{
		return true;
	}

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (a_BlockFace != BLOCK_FACE_TOP)
		{
			// Only allow planting nether wart from the top side of the block
			return false;
		}

		// Only allow placement on farmland
		int X = a_BlockX;
		int Y = a_BlockY;
		int Z = a_BlockZ;
		AddFaceDirection(X, Y, Z, a_BlockFace, true);
		if (a_World->GetBlock(X, Y, Z) != E_BLOCK_SOULSAND)
		{
			return false;
		}

		a_BlockMeta = 0;
		a_BlockType = E_BLOCK_NETHER_WART;

		return true;
	}
} ;
