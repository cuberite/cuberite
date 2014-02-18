
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

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		if (a_Dir == BLOCK_FACE_NONE)
		{
			return false;
		}

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_Dir);
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_Dir, true);

		if (Block == E_BLOCK_AIR)
		{
			int Dir = 0;
			switch (a_Dir)
			{
				case BLOCK_FACE_SOUTH: break;
				case BLOCK_FACE_NORTH: Dir = 2; break;
				case BLOCK_FACE_WEST: Dir = 1; break;
				case BLOCK_FACE_EAST: Dir = 3; break;
				default: return false;
			}

			static const struct // Define all the possible painting titles
			{
				AString Title;
			} gPaintingTitlesList[] =
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
			};

			cPainting * Painting = new cPainting(gPaintingTitlesList[a_World->GetTickRandomNumber(ARRAYCOUNT(gPaintingTitlesList) - 1)].Title, Dir, a_BlockX, a_BlockY, a_BlockZ);
			Painting->Initialize(a_World);

			if (!a_Player->IsGameModeCreative())
			{
				a_Player->GetInventory().RemoveOneEquippedItem();
			}

			return true;

		}
		return false;
	}
};




