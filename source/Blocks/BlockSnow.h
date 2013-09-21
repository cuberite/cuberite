
#pragma once

#include "BlockHandler.h"





class cBlockSnowHandler :
	public cBlockHandler
{
public:
	cBlockSnowHandler(BLOCKTYPE a_BlockType)
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
		NIBBLETYPE Meta = a_World->GetBlockMeta(Vector3i(a_BlockX, a_BlockY, a_BlockZ));

		if ((Meta < 7) && (Meta != 0)) // Is height at maximum (7) or at mininum (0)? Don't do anything if so
		{
			Meta++;
		}
		
		a_BlockMeta = Meta;
		return true;
	}


	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}
	

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SNOWBALL, 1, 0));
	}

	virtual void OnDestroyedByPlayer(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		const cItem * equipped_item = &a_Player->GetEquippedItem();
		if (!equipped_item->IsEmpty())
		{
			switch (equipped_item->m_ItemType)
			{
				case E_ITEM_DIAMOND_HOE:
				case E_ITEM_GOLD_HOE:
				case E_ITEM_IRON_HOE:
				case E_ITEM_STONE_HOE:
				case E_ITEM_WOODEN_HOE:
					cItems drops;
					Vector3i pos(a_BlockX, a_BlockY, a_BlockZ);
					ConvertToPickups(drops, a_World->GetBlockMeta(pos));
					a_World->SpawnItemPickups(drops, a_BlockX, a_BlockY, a_BlockZ);
			}
		}
	}


	virtual void OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{

	}


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return (a_RelY > 0) && g_BlockIsSnowable[a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ)];
	}
	
	
	virtual bool DoesDropOnUnsuitable(void) override
	{
		return false;
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




