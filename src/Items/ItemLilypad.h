
#pragma once

#include "ItemHandler.h"
#include "../Entities/Player.h"
#include "../LineBlockTracer.h"





class cItemLilypadHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemLilypadHandler(int a_ItemType):
		Super(a_ItemType)
	{

	}





	virtual bool IsPlaceable(void) override
	{
		return false;  // Set as not placeable so OnItemUse is called
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
		// The client sends BLOCK_FACE_NONE when it determines it should do a tracing-based placement.
		// Otherwise, a normal block face is sent.

		if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			// The position the client wants the lilypad placed.
			const auto PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);

			// Lilypad should not replace non air and non water blocks:
			if (
				const auto BlockToReplace = a_World->GetBlock(PlacePos);
				(BlockToReplace != E_BLOCK_AIR) &&
				(BlockToReplace != E_BLOCK_WATER) &&
				(BlockToReplace != E_BLOCK_STATIONARY_WATER)
			)
			{
				return false;
			}

			const auto Below = PlacePos.addedY(-1);
			if (Below.y < 0)
			{
				return false;
			}

			// Lilypad should be placed only if there is a water block below:
			if (
				const auto BlockBelow = a_World->GetBlock(Below);
				(BlockBelow != E_BLOCK_WATER) &&
				(BlockBelow != E_BLOCK_STATIONARY_WATER)
			)
			{
				return false;
			}

			a_World->SetBlock(PlacePos, E_BLOCK_LILY_PAD, 0);
			if (!a_Player->IsGameModeCreative())
			{
				a_Player->GetInventory().RemoveOneEquippedItem();
			}

			return true;
		}

		class cCallbacks:
			public cBlockTracer::cCallbacks
		{
		public:

			virtual bool OnNextBlock(Vector3i a_CBBlockPos, BLOCKTYPE a_CBBlockType, NIBBLETYPE a_CBBlockMeta, eBlockFace a_CBEntryFace) override
			{
				if (
					!IsBlockWater(a_CBBlockType) ||
					(a_CBBlockMeta != 0)  // The hit block should be a source
				)
				{
					// TODO: Vanilla stops the trace. However, we need to continue the trace, to work around our lack of block bounding box support
					// which would otherwise mean we misbehave when clicking through the voxel a (e.g.) button occupies. Now, however, we misbehave
					// when clicking on a block near water... Nonetheless, the former would cause ghost blocks, so continue for now.

					// Ignore and continue trace:
					return false;
				}

				Position = AddFaceDirection(a_CBBlockPos, BLOCK_FACE_YP);  // Always place pad at top of water block
				return true;
			}

			Vector3i Position;

		} Callbacks;

		const auto EyePosition = a_Player->GetEyePosition();
		const auto End = EyePosition + a_Player->GetLookVector() * 5;
		if (cLineBlockTracer::Trace(*a_Player->GetWorld(), Callbacks, EyePosition, End))
		{
			// The line traced to completion; no suitable water was found:
			return false;
		}

		const auto BlockToReplace = a_World->GetBlock(Callbacks.Position);
		if (BlockToReplace != E_BLOCK_AIR)
		{
			// Lilypad should not replace non air blocks:
			return false;
		}

		a_World->SetBlock(Callbacks.Position, E_BLOCK_LILY_PAD, 0);
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		return true;
	}
};
