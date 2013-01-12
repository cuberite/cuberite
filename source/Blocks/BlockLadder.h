
#pragma once

#include "BlockHandler.h"
#include "../World.h"
#include "../Ladder.h"





class cBlockLadderHandler :
	public cBlockHandler
{
public:
	cBlockLadderHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}	


	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (!LadderCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace))
		{
			a_BlockFace = FindSuitableBlockFace(a_World, a_BlockX, a_BlockY, a_BlockZ);
			
			if (a_BlockFace == BLOCK_FACE_BOTTOM)
			{
				return false;
			}
		}

		a_BlockType = m_BlockType;
		a_BlockMeta = cLadder::DirectionToMetaData(a_BlockFace);
		return true;
	}


	/// Finds a suitable Direction for the Ladder. Returns BLOCK_FACE_BOTTOM on failure
	static char FindSuitableBlockFace(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		for (int Face = 2; Face <= 5; Face++)
		{
			if (LadderCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, Face))
			{
				return Face;
			}
		}
		return BLOCK_FACE_BOTTOM;
	}
	

	static bool LadderCanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace)
	{
		if ((a_BlockFace == BLOCK_FACE_BOTTOM) || (a_BlockFace == BLOCK_FACE_TOP))
		{
			return false;
		}

		AddFaceDirection( a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);

		return g_BlockIsSolid[a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ)];
	}


	virtual bool CanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace) override
	{
		if (LadderCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace))
		{
			return true;
		}
		return (FindSuitableBlockFace(a_World, a_BlockX, a_BlockY, a_BlockZ) != BLOCK_FACE_BOTTOM);
	}


	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		char BlockFace = cLadder::MetaDataToDirection(a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ));
		return CanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, BlockFace);
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




