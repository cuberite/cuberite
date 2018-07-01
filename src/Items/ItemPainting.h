
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Entities/Painting.h"





class cItemPaintingHandler :
	public cItemHandler
{
public:
	cItemPaintingHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		if ((a_BlockFace == BLOCK_FACE_NONE) || (a_BlockFace == BLOCK_FACE_YM) || (a_BlockFace == BLOCK_FACE_YP))
		{
			// Paintings can't be flatly placed
			return false;
		}

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);  // Make sure block that will be occupied is free
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);

		if (Block == E_BLOCK_AIR)
		{
			// Define all the possible painting titles
			static const std::array<AString, 26> PaintingTitlesList =
			{
				{
					{ "Kebab" },
					{ "Aztec" },
					{ "Alban" },
					{ "Aztec2" },
					{ "Bomb" },
					{ "Plant" },
					{ "Wasteland" },
					{ "Wanderer" },
					{ "Graham" },
					{ "Pool" },
					{ "Courbet" },
					{ "Sunset" },
					{ "Sea" },
					{ "Creebet" },
					{ "Match" },
					{ "Bust" },
					{ "Stage" },
					{ "Void" },
					{ "SkullAndRoses" },
					{ "Wither" },
					{ "Fighters" },
					{ "Skeleton" },
					{ "DonkeyKong" },
					{ "Pointer" },
					{ "Pigscene" },
					{ "BurningSkull" }
				}
			};

			auto Painting = cpp14::make_unique<cPainting>(PaintingTitlesList[static_cast<size_t>(a_World->GetTickRandomNumber(PaintingTitlesList.size() - 1))], a_BlockFace, a_BlockX, a_BlockY, a_BlockZ);
			auto PaintingPtr = Painting.get();
			if (!PaintingPtr->Initialize(std::move(Painting), *a_World))
			{
				return false;
			}

			if (!a_Player->IsGameModeCreative())
			{
				a_Player->GetInventory().RemoveOneEquippedItem();
			}

			return true;

		}
		return false;
	}
};




