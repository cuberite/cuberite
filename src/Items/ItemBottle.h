
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"
#include "../World.h"





class cItemBottleHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemBottleHandler():
		Super(E_ITEM_GLASS_BOTTLE)
	{
	}





	/** Searches for a water source block in the line of sight.
	Returns true and sets a_BlockPos if a water source block is found within line-of-sight.
	Returns false if not. */
	bool GetBlockFromTrace(cWorld * a_World, cPlayer * a_Player, Vector3i & a_BlockPos)
	{
		class cCallbacks:
			public cBlockTracer::cCallbacks
		{
		public:
			Vector3i m_Pos;
			bool     m_HasHitFluid;


			cCallbacks():
				m_HasHitFluid(false)
			{
			}

			virtual bool OnNextBlock(Vector3i a_BlockPosition, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
			{
				if (IsBlockWater(a_BlockType))
				{
					if (a_BlockMeta != 0)  // we're only looking for source blocks
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
		auto Start = a_Player->GetEyePosition() + a_Player->GetLookVector();
		auto End = a_Player->GetEyePosition() + a_Player->GetLookVector() * 5;
		cLineBlockTracer::Trace(*a_World, Callbacks, Start, End);
		if (!Callbacks.m_HasHitFluid)
		{
			return false;
		}

		a_BlockPos = Callbacks.m_Pos;
		return true;
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
		if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			return false;
		}

		Vector3i BlockPos;
		if (!GetBlockFromTrace(a_World, a_Player, BlockPos))
		{
			return false;  // Nothing in range.
		}

		// Give back a filled water bottle if gamemode is not creative:
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->ReplaceOneEquippedItemTossRest(cItem(E_ITEM_POTION));
		}
		return true;
	}
} ;




