
#pragma once

#include "BlockHandler.h"





class cBlockCauldronHandler :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	cBlockCauldronHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		return cItem(E_ITEM_CAULDRON, 1, 0);
	}





	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);
		auto EquippedItem = a_Player.GetEquippedItem();
		switch (EquippedItem.m_ItemType)
		{
			case E_ITEM_BUCKET:
			{
				if (Meta == 3)
				{
					a_ChunkInterface.SetBlockMeta(a_BlockPos, 0);
					// Give new bucket, filled with fluid when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(E_ITEM_WATER_BUCKET));
					}
				}
				break;
			}
			case E_ITEM_WATER_BUCKET:
			{
				if (Meta < 3)
				{
					a_ChunkInterface.SetBlockMeta(a_BlockPos, 3);
					// Give empty bucket back when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(E_ITEM_BUCKET));
					}
				}
				break;
			}
			case E_ITEM_GLASS_BOTTLE:
			{
				if (Meta > 0)
				{
					a_ChunkInterface.SetBlockMeta(a_BlockPos, --Meta);
					// Give new potion when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(E_ITEM_POTION));
					}
				}
				break;
			}
			case E_ITEM_POTION:
			{
				// Refill cauldron with water bottles.
				if ((Meta < 3) && (EquippedItem.m_ItemDamage == 0))
				{
					a_ChunkInterface.SetBlockMeta(Vector3i(a_BlockPos), ++Meta);
					// Give back an empty bottle when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(E_ITEM_GLASS_BOTTLE));
					}
				}
			}
		}
		return true;
	}





	virtual bool IsUseable() override
	{
		return true;
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) override
	{
		auto WorldPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		if (!a_WorldInterface.IsWeatherWetAtXYZ(WorldPos.addedY(1)))
		{
			// It's not raining at our current location or we do not have a direct view of the sky
			return;
		}

		auto Meta = a_Chunk.GetMeta(a_RelPos);
		if (Meta < 3)
		{
			a_Chunk.SetMeta(a_RelPos, Meta + 1);
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 21;
	}
} ;




