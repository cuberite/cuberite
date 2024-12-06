
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../Simulator/FluidSimulator.h"
#include "../Blocks/BlockHandler.h"
#include "../LineBlockTracer.h"
#include "../Blocks/ChunkInterface.h"





class cItemBucketHandler final :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) const override
	{
		switch (m_ItemType)
		{
			case Item::Bucket:      return ScoopUpFluid(a_World, a_Player, a_HeldItem, a_ClickedBlockPos, a_ClickedBlockFace);
			case Item::LavaBucket:  return PlaceFluid  (a_World, a_Player, a_PluginInterface, a_HeldItem, a_ClickedBlockPos, a_ClickedBlockFace, Block::Lava::Lava());
			case Item::WaterBucket: return PlaceFluid  (a_World, a_Player, a_PluginInterface, a_HeldItem, a_ClickedBlockPos, a_ClickedBlockFace, Block::Water::Water());
			case Item::PowderSnowBucket: return PlaceFluid  (a_World, a_Player, a_PluginInterface, a_HeldItem, a_ClickedBlockPos, a_ClickedBlockFace, Block::PowderSnow::PowderSnow());  // not really a fluid but it works
			default:
			{
				FLOGWARNING("{}: Item type not handled {}.", __FUNCTION__, m_ItemType);
				return false;
			}
		}
	}





	bool ScoopUpFluid(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, const Vector3i a_ClickedBlockPos, eBlockFace a_ClickedBlockFace) const
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

		if (cBlockFluidHandler::GetFalloff(a_World->GetBlock(BlockPos)) != 0)
		{
			// Not a source block
			return false;
		}

		auto Block = a_World->GetBlock(BlockPos);
		enum Item NewItemType;

		if (Block.Type() == BlockType::Water)
		{
			NewItemType = Item::WaterBucket;
		}
		else if (Block.Type() == BlockType::Lava)
		{
			NewItemType = Item::LavaBucket;
		}
		else if (Block.Type() == BlockType::PowderSnow)
		{
			NewItemType = Item::PowderSnowBucket;
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

		// Remove water / lava block (unless plugins disagree):
		if (!a_Player->PlaceBlock(BlockPos, Block::Air::Air()))
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
		const Vector3i a_BlockPos, eBlockFace a_BlockFace, BlockState a_FluidBlock
	) const
	{
		// Players can't place fluid while in adventure mode.
		if (a_Player->IsGameModeAdventure())
		{
			return false;
		}

		if (a_BlockFace != BLOCK_FACE_NONE)
		{
			// return false;
		}

		BlockState Dest;
		eBlockFace EntryFace;
		Vector3i BlockPos;
		if (!GetPlacementCoordsFromTrace(a_World, a_Player, BlockPos, Dest, EntryFace))
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
			a_Player->ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
		}

		// Wash away anything that was there prior to placing:
		if (cFluidSimulator::CanWashAway(Dest))
		{
			if (a_PluginInterface.CallHookPlayerBreakingBlock(*a_Player, BlockPos, EntryFace, Dest))
			{
				// Plugin disagrees with the washing-away
				return false;
			}
			a_World->DropBlockAsPickups(BlockPos, a_Player, nullptr);
			a_PluginInterface.CallHookPlayerBrokenBlock(*a_Player, BlockPos, EntryFace, Dest);
		}

		// Place the actual fluid block:
		return a_Player->PlaceBlock(BlockPos, a_FluidBlock);
	}





	bool GetBlockFromTrace(cWorld * a_World, cPlayer * a_Player, Vector3i & a_BlockPos) const
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

			virtual bool OnNextBlock(Vector3i a_BlockPosition, BlockState a_Block, eBlockFace a_EntryFace) override
			{
				if (cBlockFluidHandler::IsBlockLiquid(a_Block))
				{
					if (cBlockFluidHandler::GetFalloff(a_Block) != 0)  // GetBlockFromTrace is called for scooping up fluids; the hit block should be a source
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





	bool GetPlacementCoordsFromTrace(cWorld * a_World, cPlayer * a_Player, Vector3i & a_BlockPos, BlockState & a_Block, eBlockFace & a_BlockFace) const
	{
		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:
			Vector3i   m_Pos;
			BlockState m_ReplacedBlock;
			eBlockFace m_EntryFace;

			virtual bool OnNextBlock(Vector3i a_CBBlockPos, BlockState a_CBBlock, eBlockFace a_CBEntryFace) override
			{
				if ((a_CBBlock != Block::Air::Air()) && !IsBlockLiquid(a_CBBlock))
				{
					m_ReplacedBlock = a_CBBlock;
					m_EntryFace = a_CBEntryFace;
					if (!cFluidSimulator::CanWashAway(a_CBBlock))
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
			a_Block = Callbacks.m_ReplacedBlock;
			a_BlockFace = Callbacks.m_EntryFace;
			return true;
		}

		return false;
	}
};
