
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../Simulator/FluidSimulator.h"
#include "../Blocks/BlockHandler.h"
#include "../LineBlockTracer.h"
#include "../Blocks/ChunkInterface.h"





class cItemBucketHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemBucketHandler(int a_ItemType):
		Super(a_ItemType)
	{

	}





	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_BUCKET:       return ScoopUpFluid(a_World, a_Player, a_HeldItem, a_ClickedBlockPos, a_ClickedBlockFace);
			case E_ITEM_LAVA_BUCKET:  return PlaceFluid  (a_World, a_Player, a_PluginInterface, a_HeldItem, a_ClickedBlockPos, a_ClickedBlockFace, E_BLOCK_LAVA);
			case E_ITEM_WATER_BUCKET: return PlaceFluid  (a_World, a_Player, a_PluginInterface, a_HeldItem, a_ClickedBlockPos, a_ClickedBlockFace, E_BLOCK_WATER);
			default:
			{
				ASSERT(!"Unhandled ItemType");
				return false;
			}
		}
	}





	bool ScoopUpFluid(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, const Vector3i a_ClickedBlockPos, eBlockFace a_ClickedBlockFace)
	{
		// Players can't pick up fluid while in adventure mode.
		if (a_Player->IsGameModeAdventure())
		{
			return false;
		}

		// Needs a valid clicked block:
		if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			return false;
		}

		Vector3i BlockPos;
		if (!GetBlockFromTrace(a_World, a_Player, BlockPos))
		{
			return false;  // Nothing in range.
		}

		if (a_World->GetBlockMeta(BlockPos.x, BlockPos.y, BlockPos.z) != 0)
		{
			// Not a source block
			return false;
		}

		BLOCKTYPE Block = a_World->GetBlock(BlockPos.x, BlockPos.y, BlockPos.z);
		ENUM_ITEM_TYPE NewItemType;

		if (IsBlockWater(Block))
		{
			NewItemType = E_ITEM_WATER_BUCKET;
		}
		else if (IsBlockLava(Block))
		{
			NewItemType = E_ITEM_LAVA_BUCKET;
		}
		else
		{
			return false;
		}

		// Check to see if destination block is too far away
		// Reach Distance Multiplayer = 5 Blocks
		if ((BlockPos.x - a_Player->GetPosX() > 5) || (BlockPos.z - a_Player->GetPosZ() > 5))
		{
			return false;
		}

		// Remove water / lava block (unless plugins disagree)
		if (!a_Player->PlaceBlock(BlockPos.x, BlockPos.y, BlockPos.z, E_BLOCK_AIR, 0))
		{
			return false;
		}

		// Give new bucket, filled with fluid when the gamemode is not creative:
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->ReplaceOneEquippedItemTossRest(cItem(NewItemType));
		}

		return true;
	}





	bool PlaceFluid(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		const Vector3i a_BlockPos, eBlockFace a_BlockFace, BLOCKTYPE a_FluidBlock
	)
	{
		// Players can't place fluid while in adventure mode.
		if (a_Player->IsGameModeAdventure())
		{
			return false;
		}

		if (a_BlockFace != BLOCK_FACE_NONE)
		{
			return false;
		}

		BLOCKTYPE CurrentBlockType;
		NIBBLETYPE CurrentBlockMeta;
		eBlockFace EntryFace;
		Vector3i BlockPos;
		if (!GetPlacementCoordsFromTrace(a_World, a_Player, BlockPos, CurrentBlockType, CurrentBlockMeta, EntryFace))
		{
			return false;
		}

		// Check to see if destination block is too far away
		// Reach Distance Multiplayer = 5 Blocks
		if ((BlockPos.x - a_Player->GetPosX() > 5) || (BlockPos.z - a_Player->GetPosZ() > 5))
		{
			return false;
		}

		// Give back an empty bucket if the gamemode is not creative:
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->ReplaceOneEquippedItemTossRest(cItem(E_ITEM_BUCKET));
		}

		// Wash away anything that was there prior to placing:
		if (cFluidSimulator::CanWashAway(CurrentBlockType))
		{
			if (a_PluginInterface.CallHookPlayerBreakingBlock(*a_Player, BlockPos.x, BlockPos.y, BlockPos.z, EntryFace, CurrentBlockType, CurrentBlockMeta))
			{
				// Plugin disagrees with the washing-away
				return false;
			}
			a_World->DropBlockAsPickups(BlockPos, a_Player, nullptr);
			a_PluginInterface.CallHookPlayerBrokenBlock(*a_Player, BlockPos.x, BlockPos.y, BlockPos.z, EntryFace, CurrentBlockType, CurrentBlockMeta);
		}

		// Place the actual fluid block:
		return a_Player->PlaceBlock(BlockPos.x, BlockPos.y, BlockPos.z, a_FluidBlock, 0);
	}





	bool GetBlockFromTrace(cWorld * a_World, cPlayer * a_Player, Vector3i & a_BlockPos)
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

			virtual bool OnNextBlock(Vector3i a_BlockPosition, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
			{
				if (IsBlockWater(a_BlockType) || IsBlockLava(a_BlockType))
				{
					if (a_BlockMeta != 0)  // GetBlockFromTrace is called for scooping up fluids; the hit block should be a source
					{
						return false;
					}
					m_HasHitFluid = true;
					m_Pos = a_BlockPosition;
					return true;
				}
				return false;
			}
		} Callbacks;

		cLineBlockTracer Tracer(*a_World, Callbacks);
		Vector3d Start(a_Player->GetEyePosition() + a_Player->GetLookVector());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		Tracer.Trace(Start, End);

		if (!Callbacks.m_HasHitFluid)
		{
			return false;
		}


		a_BlockPos = Callbacks.m_Pos;
		return true;
	}





	bool GetPlacementCoordsFromTrace(cWorld * a_World, cPlayer * a_Player, Vector3i & a_BlockPos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta, eBlockFace & a_BlockFace)
	{
		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:
			Vector3i   m_Pos;
			BLOCKTYPE  m_ReplacedBlockType;
			NIBBLETYPE m_ReplacedBlockMeta;
			eBlockFace m_EntryFace;

			virtual bool OnNextBlock(Vector3i a_CBBlockPos, BLOCKTYPE a_CBBlockType, NIBBLETYPE a_CBBlockMeta, eBlockFace a_CBEntryFace) override
			{
				if ((a_CBBlockType != E_BLOCK_AIR) && !IsBlockLiquid(a_CBBlockType))
				{
					m_ReplacedBlockType = a_CBBlockType;
					m_ReplacedBlockMeta = a_CBBlockMeta;
					m_EntryFace = static_cast<eBlockFace>(a_CBEntryFace);
					if (!cFluidSimulator::CanWashAway(a_CBBlockType))
					{
						a_CBBlockPos = AddFaceDirection(a_CBBlockPos, a_CBEntryFace);  // Was an unwashawayable block, can't overwrite it!
					}
					m_Pos = a_CBBlockPos;  // (Block could be washed away, replace it)
					return true;  // Abort tracing
				}
				return false;
			}
		} Callbacks;

		cLineBlockTracer Tracer(*a_World, Callbacks);
		Vector3d Start(a_Player->GetEyePosition());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		// cLineBlockTracer::Trace() returns true when whole line was traversed. By returning true from the callback when we hit something,
		// we ensure that this never happens if liquid could be placed
		// Use this to judge whether the position is valid
		if (!Tracer.Trace(Start, End))
		{
			a_BlockPos = Callbacks.m_Pos;
			a_BlockType = Callbacks.m_ReplacedBlockType;
			a_BlockMeta = Callbacks.m_ReplacedBlockMeta;
			a_BlockFace = Callbacks.m_EntryFace;
			return true;
		}

		return false;
	}
};
