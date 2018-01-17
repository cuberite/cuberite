
#pragma once

#include "BlockEntity.h"
#include "../BlockEntities/MobHeadEntity.h"





class cBlockMobHeadHandler :
	public cBlockEntityHandler
{
public:
	cBlockMobHeadHandler(BLOCKTYPE a_BlockType):
		cBlockEntityHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// The drop spawn is in the OnDestroyedByPlayer method
	}

	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		if (a_Player.IsGameModeCreative())
		{
			// No drops in creative mode
			return;
		}

		a_WorldInterface.DoWithBlockEntityAt(a_BlockX, a_BlockY, a_BlockZ, [](cBlockEntity & a_BlockEntity)
			{
				if (a_BlockEntity.GetBlockType() != E_BLOCK_HEAD)
				{
					return false;
				}
				auto & MobHeadEntity = static_cast<cMobHeadEntity&>(a_BlockEntity);

				cItems Pickups;
				Pickups.Add(E_ITEM_HEAD, 1, static_cast<short>(MobHeadEntity.GetType()));
				auto & r1 = GetRandomProvider();

				// Mid-block position first
				double MicroX, MicroY, MicroZ;
				MicroX = MobHeadEntity.GetPosX() + 0.5;
				MicroY = MobHeadEntity.GetPosY() + 0.5;
				MicroZ = MobHeadEntity.GetPosZ() + 0.5;

				// Add random offset second
				MicroX += r1.RandReal<double>(-0.5, 0.5);
				MicroZ += r1.RandReal<double>(-0.5, 0.5);

				MobHeadEntity.GetWorld()->SpawnItemPickups(Pickups, MicroX, MicroY, MicroZ);
				return false;
			}
		);
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




