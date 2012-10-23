
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


	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		if (!LadderCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Dir))
		{
			a_Dir = FindSuitableDirection(a_World, a_BlockX, a_BlockY, a_BlockZ);
			
			if (a_Dir == BLOCK_FACE_BOTTOM)
			{
				return;
			}
		}

		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, cLadder::DirectionToMetaData(a_Dir));
		OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
	}

	/// Finds a suitable Direction for the Ladder. Returns BLOCK_FACE_BOTTOM on failure
	static char FindSuitableDirection(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		for (int i = 2; i <= 5; i++)
		{
			if (LadderCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, i))
			{
				return i;
			}
		}
		return BLOCK_FACE_BOTTOM;
	}
	

	static bool LadderCanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
	{
		if (a_Dir == BLOCK_FACE_BOTTOM || a_Dir == BLOCK_FACE_TOP )
		{
			return false;
		}

		AddDirection( a_BlockX, a_BlockY, a_BlockZ, a_Dir, true );

		return g_BlockIsSolid[a_World->GetBlock( a_BlockX, a_BlockY, a_BlockZ)];
	}


	virtual bool CanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		if (LadderCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, a_Dir))
			return true;
		return FindSuitableDirection(a_World, a_BlockX, a_BlockY, a_BlockZ) != BLOCK_FACE_BOTTOM;
	}


	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		char Dir = cLadder::MetaDataToDirection(a_World->GetBlockMeta( a_BlockX, a_BlockY, a_BlockZ));
		return CanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, Dir);
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




