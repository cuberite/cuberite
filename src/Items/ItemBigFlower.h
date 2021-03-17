
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


	virtual bool GetBlocksToPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		sSetBlockVector & a_BlocksToPlace
	) override
	{
		// Can only be placed on dirt:
		if ((a_PlacedBlockPos.y <= 0) || !IsBlockStateOfDirt(a_World.GetBlock(a_PlacedBlockPos.addedY(-1))))
		{
			return false;
		}

		// Needs at least two free blocks to build in
		if (a_PlacedBlockPos.y >= cChunkDef::Height - 1)
		{
			return false;
		}

		auto TopPos = a_PlacedBlockPos.addedY(1);
		auto TopBlock = a_World.GetBlock(TopPos);
		cChunkInterface ChunkInterface(a_World.GetChunkMap());

		if (!cBlockHandler::For(TopBlock.Type()).DoesIgnoreBuildCollision(ChunkInterface, TopPos, a_Player, TopBlock))
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

		return true;
	}
};
