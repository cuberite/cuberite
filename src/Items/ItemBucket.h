
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Simulator/FluidSimulator.h"
#include "../Blocks/BlockHandler.h"
#include "../LineBlockTracer.h"





class cItemBucketHandler :
	public cItemHandler
{
public:
	cItemBucketHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_BUCKET:       return ScoopUpFluid(a_World, a_Player, a_Item, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
			case E_ITEM_LAVA_BUCKET:  return PlaceFluid  (a_World, a_Player, a_Item, a_BlockX, a_BlockY, a_BlockZ, a_Dir, E_BLOCK_LAVA);
			case E_ITEM_WATER_BUCKET: return PlaceFluid  (a_World, a_Player, a_Item, a_BlockX, a_BlockY, a_BlockZ, a_Dir, E_BLOCK_WATER);
			default:
			{
				ASSERT(!"Unhandled ItemType");
				return false;
			}
		}
	}
	
	
	
	bool ScoopUpFluid(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace)
	{
		if (a_BlockFace > 0)
		{
			return false;
		}

		Vector3i BlockPos;
		if (!GetBlockFromTrace(a_World, a_Player, BlockPos))
		{
			return false; // Nothing in range.
		}

		if (a_World->GetBlockMeta(BlockPos.x, BlockPos.y, BlockPos.z) != 0)
		{
			// Not a source block
			return false;
		}

		BLOCKTYPE Block = a_World->GetBlock(BlockPos.x, BlockPos.y, BlockPos.z);
		ENUM_ITEM_ID NewItem;

		if (IsBlockWater(Block))
		{
			NewItem = E_ITEM_WATER_BUCKET;
		}
		else if (IsBlockLava(Block))
		{
			NewItem = E_ITEM_LAVA_BUCKET;
		}
		else
		{
			return false;
		}
		
		// Give new bucket, filled with fluid when the gamemode is not creative:
		if (!a_Player->IsGameModeCreative())
		{
			// Remove the bucket from the inventory
			if (!a_Player->GetInventory().RemoveOneEquippedItem())
			{
				LOG("Clicked with an empty bucket, but cannot remove one from the inventory? WTF?");
				ASSERT(!"Inventory bucket mismatch");
				return true;
			}
			a_Player->GetInventory().AddItem(cItem(NewItem), true, true);
		}

		// Remove water / lava block
		a_Player->GetWorld()->SetBlock(BlockPos.x, BlockPos.y, BlockPos.z, E_BLOCK_AIR, 0);
		return true;
	}


	bool PlaceFluid(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_FluidBlock)
	{			
		if (a_BlockFace < 0)
		{
			return false;
		}
		
		BLOCKTYPE CurrentBlock = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		bool CanWashAway = cFluidSimulator::CanWashAway(CurrentBlock);
		if (!CanWashAway)
		{
			// The block pointed at cannot be washed away, so put fluid on top of it / on its sides
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			CurrentBlock = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		}
		if (
			!CanWashAway && 
			(CurrentBlock != E_BLOCK_AIR) && 
			(CurrentBlock != E_BLOCK_WATER) &&
			(CurrentBlock != E_BLOCK_STATIONARY_WATER) &&
			(CurrentBlock != E_BLOCK_LAVA) &&
			(CurrentBlock != E_BLOCK_STATIONARY_LAVA)
		)
		{
			// Cannot place water here
			return false;
		}
		
		if (a_Player->GetGameMode() != gmCreative)
		{
			// Remove fluid bucket, add empty bucket:
			if (!a_Player->GetInventory().RemoveOneEquippedItem())
			{
				LOG("Clicked with a full bucket, but cannot remove one from the inventory? WTF?");
				ASSERT(!"Inventory bucket mismatch");
				return false;
			}
			cItem Item(E_ITEM_BUCKET, 1);
			if (!a_Player->GetInventory().AddItem(Item,true,true))
			{
				return false;
			}
		}
		
		// Wash away anything that was there prior to placing:
		if (CanWashAway)
		{
			cBlockHandler * Handler = BlockHandler(CurrentBlock);
			if (Handler->DoesDropOnUnsuitable())
			{
				Handler->DropBlock(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ);
			}
		}

		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, a_FluidBlock, 0);
		
		return true;
	}


	bool GetBlockFromTrace(cWorld * a_World, cPlayer * a_Player, Vector3i & BlockPos)
	{
		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:
			Vector3i m_Pos;
			bool     m_HasHitFluid;
			

			cCallbacks(void) :
				m_HasHitFluid(false)
			{
			}
			
			virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, char a_EntryFace) override
			{
				if (a_BlockMeta != 0)  // Even if it was a water block it would not be a source.
				{
					return false;
				}
				if (IsBlockWater(a_BlockType) || IsBlockLava(a_BlockType))
				{
					m_HasHitFluid = true;
					m_Pos.Set(a_BlockX, a_BlockY, a_BlockZ);
					return true;
				}
				return false;
			}
		} Callbacks;

		cLineBlockTracer Tracer(*a_World, Callbacks);
		Vector3d Start(a_Player->GetEyePosition() + a_Player->GetLookVector());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		Tracer.Trace(Start.x, Start.y, Start.z, End.x, End.y, End.z);

		if (!Callbacks.m_HasHitFluid)
		{
			return false;
		}


		BlockPos.Set(Callbacks.m_Pos.x, Callbacks.m_Pos.y, Callbacks.m_Pos.z);
		return true;
	}

};
