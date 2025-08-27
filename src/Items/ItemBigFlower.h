
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"





class cItemBigFlowerHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		// Needs at least two free blocks to build in:
		if (a_PlacePosition.y >= (cChunkDef::Height - 1))
		{
			return false;
		}

		const auto & World = *a_Player.GetWorld();
		const auto TopPos = a_PlacePosition.addedY(1);
		BlockState BlockToReplace;
		if (World.GetBlock(TopPos, BlockToReplace))
		{
			return false;
		}

		if (!cBlockHandler::For(BlockToReplace.Type()).DoesIgnoreBuildCollision(World, a_HeldItem, TopPos, BlockToReplace, a_ClickedBlockFace, false))
		{
			return false;
		}

		using namespace Block;

		switch (a_HeldItem.m_ItemType)
		{
			case Item::Sunflower:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, Sunflower::Sunflower(Sunflower::Half::Lower) },
					{ TopPos,          Sunflower::Sunflower(Sunflower::Half::Upper) }
				});
			}
			case Item::Lilac:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, Lilac::Lilac(Lilac::Half::Lower) },
					{ TopPos,           Lilac::Lilac(Lilac::Half::Upper) }
				});
			}
			case Item::TallGrass:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, TallGrass::TallGrass(TallGrass::Half::Lower) },
					{ TopPos,          TallGrass::TallGrass(TallGrass::Half::Upper) }
				});
			}
			case Item::LargeFern:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, LargeFern::LargeFern(LargeFern::Half::Lower) },
					{ TopPos,          LargeFern::LargeFern(LargeFern::Half::Upper) }
				});
			}
			case Item::RoseBush:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, RoseBush::RoseBush(RoseBush::Half::Lower) },
					{ TopPos,          RoseBush::RoseBush(RoseBush::Half::Upper) }
				});
			}
			case Item::Peony:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, Peony::Peony(Peony::Half::Lower) },
					{ TopPos,          Peony::Peony(Peony::Half::Upper) }
				});
			}
			default: return false;
		}
	}
};
