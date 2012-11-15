
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
	
	
	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, cVine::DirectionToMetaData(a_Dir));
		OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
	}


	virtual bool CanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		if (
			(a_World->GetBlock( a_BlockX, a_BlockY + 1, a_BlockZ ) == E_BLOCK_VINES) &&
			(cVine::MetaDataToDirection(a_World->GetBlockMeta( a_BlockX, a_BlockY + 1, a_BlockZ )) == a_Dir)
		)
		{
			return true;
		}
		
		BLOCKTYPE TopBlock = a_World->GetBlock( a_BlockX, a_BlockY + 1, a_BlockZ);
		if (g_BlockIsSolid[TopBlock] || TopBlock==E_BLOCK_LEAVES)
		{
			AddDirection( a_BlockX, a_BlockY, a_BlockZ, a_Dir, true );
			BLOCKTYPE BaseBlock = a_World->GetBlock( a_BlockX, a_BlockY, a_BlockZ);
			if (g_BlockIsSolid[BaseBlock] == false && BaseBlock!=E_BLOCK_LEAVES)
			{
				AddDirection( a_BlockX, a_BlockY, a_BlockZ, a_Dir, false );
				a_World->SetBlock( a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_VINES, 0);
			}
			return true;
		}

		AddDirection( a_BlockX, a_BlockY, a_BlockZ, a_Dir, true );
		BLOCKTYPE BaseBlock = a_World->GetBlock( a_BlockX, a_BlockY, a_BlockZ);
		return g_BlockIsSolid[BaseBlock] || BaseBlock==E_BLOCK_LEAVES;
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




