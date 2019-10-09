
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"
#include "../Item.h"
#include "../BlockEntities/BlockEntityWithItems.h"





/** Wrapper for blocks that have a cBlockEntity descendant attached to them and can be "used" by the player.
Forwards the "use" event to the block entity. */
class cBlockEntityHandler:
	public cBlockHandler
{
	using super = cBlockHandler;

public:

	cBlockEntityHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		return a_ChunkInterface.UseBlockEntity(&a_Player, a_BlockX, a_BlockY, a_BlockZ);
	}





	virtual bool IsUseable() override
	{
		return true;
	}
};





/** Wrapper for blocks that have a cBlockEntityWithItems descendant attached to them.
When converting to pickups, drops self with meta reset to zero, and adds the container contents. */
template <typename Base = cBlockEntityHandler>
class cContainerEntityHandler:
	public Base
{
public:

	cContainerEntityHandler(BLOCKTYPE a_BlockType):
		Base(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Reset meta to 0
		cItems res(cItem(Base::m_BlockType, 1, 0));

		// Drop the contents:
		if (a_BlockEntity != nullptr)
		{
			auto container = static_cast<cBlockEntityWithItems *>(a_BlockEntity);
			res.AddItemGrid(container->GetContents());
		}
		return res;
	}
};
