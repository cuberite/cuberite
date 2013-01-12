
#pragma once

#include "BlockHandler.h"
#include "../Vine.h"





class cBlockVineHandler :
	public cBlockHandler
{
public:
	cBlockVineHandler(BLOCKTYPE a_BlockType)
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
		a_BlockType = m_BlockType;
		a_BlockMeta = cVine::DirectionToMetaData(a_BlockFace);
		return true;
	}


	virtual bool CanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace) override
	{
		if (
			(a_World->GetBlock( a_BlockX, a_BlockY + 1, a_BlockZ ) == E_BLOCK_VINES) &&
			(cVine::MetaDataToDirection(a_World->GetBlockMeta(a_BlockX, a_BlockY + 1, a_BlockZ)) == a_BlockFace)
		)
		{
			return true;
		}
		
		BLOCKTYPE TopBlock = a_World->GetBlock( a_BlockX, a_BlockY + 1, a_BlockZ);
		if (g_BlockIsSolid[TopBlock] || (TopBlock == E_BLOCK_LEAVES))
		{
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);
			BLOCKTYPE BaseBlock = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
			if (!g_BlockIsSolid[BaseBlock] && (BaseBlock != E_BLOCK_LEAVES))
			{
				AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, false);
				a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_VINES, 0);
			}
			return true;
		}

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);
		BLOCKTYPE BaseBlock = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		return (g_BlockIsSolid[BaseBlock] || (BaseBlock == E_BLOCK_LEAVES));
	}


	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		char Dir = cVine::MetaDataToDirection(a_World->GetBlockMeta( a_BlockX, a_BlockY, a_BlockZ));
		return CanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, Dir);
	}


	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}
	
	
	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}


	bool DoesDropOnUnsuitable(void)
	{
		return false;
	}
} ;




