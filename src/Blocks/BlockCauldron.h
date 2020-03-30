
#pragma once

#include "BlockHandler.h"





class cBlockCauldronHandler :
	public cBlockHandler
{
	using super = cBlockHandler;

public:

	cBlockCauldronHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		return cItem(E_ITEM_CAULDRON, 1, 0);
	}

	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta({a_BlockX, a_BlockY, a_BlockZ});
		auto EquippedItem = a_Player.GetEquippedItem();
		switch (EquippedItem.m_ItemType)
		{
			case E_ITEM_BUCKET:
			{
				if (Meta == 3)
				{
					a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 0);
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
					a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 3);
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
					a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, --Meta);
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
					a_ChunkInterface.SetBlockMeta(Vector3i(a_BlockX, a_BlockY, a_BlockZ), ++Meta);
					// Give empty bottle when the gamemode is not creative:
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

	virtual void OnUpdate(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
		if (!a_WorldInterface.IsWeatherWetAt(BlockX, BlockZ) || (a_RelY != a_WorldInterface.GetHeight(BlockX, BlockZ)))
		{
			// It's not raining at our current location or we do not have a direct view of the sky
			// We cannot eat the rain :(
			return;
		}

		NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
		if (Meta < 3)
		{
			a_Chunk.SetMeta(a_RelX, a_RelY, a_RelZ, Meta + 1);
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 21;
	}
} ;




