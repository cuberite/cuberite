
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"





class cItemBigFlowerHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	cItemBigFlowerHandler():
		Super(E_BLOCK_BIG_FLOWER)
	{
	}





	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		// Needs at least two free blocks to build in:
		if (a_PlacePosition.y >= (cChunkDef::Height - 1))
		{
			return false;
		}

		const auto & World = *a_Player.GetWorld();
		const auto TopPos = a_PlacePosition.addedY(1);
		BLOCKTYPE TopType;
		NIBBLETYPE TopMeta;
		World.GetBlockTypeMeta(TopPos, TopType, TopMeta);

		if (!cBlockHandler::For(TopType).DoesIgnoreBuildCollision(World, a_HeldItem, TopPos, TopMeta, a_ClickedBlockFace, false))
		{
			return false;
		}

		using namespace Block;

		switch (a_EquippedItem.m_ItemDamage)
		{
			case E_META_BIG_FLOWER_SUNFLOWER:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, Sunflower::Sunflower(Sunflower::Half::Lower));
				a_BlocksToPlace.emplace_back(TopPos,           Sunflower::Sunflower(Sunflower::Half::Upper));
				break;
			}
			case E_META_BIG_FLOWER_LILAC:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, Lilac::Lilac(Lilac::Half::Lower));
				a_BlocksToPlace.emplace_back(TopPos,           Lilac::Lilac(Lilac::Half::Upper));
				break;
			}
			case E_META_BIG_FLOWER_DOUBLE_TALL_GRASS:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, TallGrass::TallGrass(TallGrass::Half::Lower));
				a_BlocksToPlace.emplace_back(TopPos,           TallGrass::TallGrass(TallGrass::Half::Upper));
				break;
			}
			case E_META_BIG_FLOWER_LARGE_FERN:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, LargeFern::LargeFern(LargeFern::Half::Lower));
				a_BlocksToPlace.emplace_back(TopPos,           LargeFern::LargeFern(LargeFern::Half::Upper));
				break;
			}
			case E_META_BIG_FLOWER_ROSE_BUSH:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, RoseBush::RoseBush(RoseBush::Half::Lower));
				a_BlocksToPlace.emplace_back(TopPos,           RoseBush::RoseBush(RoseBush::Half::Upper));
				break;
			}
			case E_META_BIG_FLOWER_PEONY:
			{
				a_BlocksToPlace.emplace_back(a_PlacedBlockPos, Peony::Peony(Peony::Half::Lower));
				a_BlocksToPlace.emplace_back(TopPos,           Peony::Peony(Peony::Half::Upper));
				break;
			}
			default: return false;
		}

		return a_Player.PlaceBlocks(
				{
						{ a_PlacePosition, E_BLOCK_BIG_FLOWER, static_cast<NIBBLETYPE>(a_HeldItem.m_ItemDamage & 0x07) },
						{ TopPos,          E_BLOCK_BIG_FLOWER, E_META_BIG_FLOWER_TOP }
				});

		return true;
	}
};
1