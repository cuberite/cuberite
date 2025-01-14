
#pragma once

#include "ItemHandler.h"
#include "../Entities/Player.h"
#include "../LineBlockTracer.h"





class cItemLilypadHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual bool IsPlaceable(void) const override
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
	) const override
	{
		// The client sends BLOCK_FACE_NONE when it determines it should do a tracing-based placement.
		// Otherwise, a normal block face is sent.

		if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			// The position the client wants the lilypad placed.
			const auto PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);

			// Lilypad should not replace non air and non water blocks:
			if (
				auto BlockToReplace = a_World->GetBlock(PlacePos);
				(BlockToReplace.Type() != BlockType::Air) &&
				(BlockToReplace.Type() != BlockType::Water)
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
				auto BlockBelow = a_World->GetBlock(Below);
				(BlockBelow.Type() != BlockType::Water)
			)
			{
				return false;
			}

			a_World->SetBlock(PlacePos, Block::LilyPad::LilyPad());
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

			virtual bool OnNextBlock(Vector3i a_CBBlockPos, BlockState a_CBBlock, eBlockFace a_CBEntryFace) override
			{
				if (
					(a_CBBlock.Type() != BlockType::Water) &&
					(cBlockFluidHandler::GetFalloff(a_CBBlock) != 0)  // The hit block should be a source
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
		if (BlockToReplace.Type() != BlockType::Air)
		{
			// Lilypad should not replace non air blocks:
			return false;
		}

		a_World->SetBlock(Callbacks.Position, Block::LilyPad::LilyPad());
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		return true;
	}
};
