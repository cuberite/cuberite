
#pragma once

#include "BlockHandler.h"
#include "../UI/CraftingWindow.h"
#include "../Entities/Player.h"





class cBlockCraftingTableHandler final :
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
		a_Player.GetStatistics().Custom[CustomStatistic::InteractWithCraftingTable]++;

		cWindow * Window = new cCraftingWindow();
		a_Player.OpenWindow(*Window);
		return true;
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 13;
	}
} ;




