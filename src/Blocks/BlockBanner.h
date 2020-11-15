
// BlockBanner.h

#pragma once

#include "../BlockInfo.h"

/*
	may either be placed on top of a block or on the side (may overlap)
	has no collisions
	hitbox is one block tall
	water and lava flow around banners (water goes through the bottom part if placed onto a wall)
	lava can spawn fire blocks next to the banner but doesn't burn the banner
	cannot be moved by pistons
	breaks if block below is broken

	sounds:
		Description                            |sound             | Volume | Pitch |dist
		Breaking the block                      block.wood.break    1.0     0.8     16
		Falling on the block with fall damage   block.wood.fall	    0.5     0.75    16
		Mining the block                        block.wood.hit      0.25    0.5     16
		Placing the block                       block.wood.place    1.0     0.8     16
		Walking on the block                    block.wood.step	    0.15    1.0     16
*/


class cBlockBannerHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (!cChunkDef::IsValidRelPos(a_RelPos))
		{
			return false;
		}
		BLOCKTYPE Type = a_Chunk.GetBlock(a_RelPos.addedY(-1));
		return (cBlockInfo::IsSolid(Type));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;
