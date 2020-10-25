
#pragma once

#include "BlockHandler.h"
#include "../Item.h"
#include "../UI/EnchantingWindow.h"
#include "../BlockEntities/EnchantingTableEntity.h"
#include "../Entities/Player.h"





class cBlockEnchantingTableHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		AString WindowName = "Enchant";
		a_WorldInterface.DoWithBlockEntityAt(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, [&WindowName](cBlockEntity & a_Entity)
		{
			if (a_Entity.GetBlockType() != E_BLOCK_ENCHANTMENT_TABLE)
			{
				return false;
			}

			const auto & EnchantingTable = static_cast<cEnchantingTableEntity &>(a_Entity);
			const auto & CustomName = EnchantingTable.GetCustomName();
			if (!CustomName.empty())
			{
				WindowName = CustomName;
			}

			return true;
		});

		cWindow * Window = new cEnchantingWindow(a_BlockPos, std::move(WindowName));
		a_Player.OpenWindow(*Window);

		return true;
	}


	virtual bool IsUseable(void) const override
	{
		return true;
	}


	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}


	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 29;
	}
};
