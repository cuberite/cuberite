
// BlockBanner.h

#pragma once

#include "BlockEntity.h"
#include "Mixins.h"
#include "ChunkInterface.h"


class cEntity;
class cPlayer;
class cWorldInterface;

/*
	may either be placed on top of a block or on the side (may overlap)
	has no collisions
	hitbox is one block tall
	water and lava flow around banners (water goes through the bottom part if placed onto a wall)
	lava can spawn fire blocks next to the banner but doesn't burn the banner
	cannot be moved by pistons

	sounds:
		Description                            |sound             | Volume | Pitch |dist
		Breaking the block                      block.wood.break    1.0     0.8     16
		Falling on the block with fall damage   block.wood.fall	    0.5     0.75    16
		Mining the block                        block.wood.hit      0.25    0.5     16
		Placing the block                       block.wood.place    1.0     0.8     16
		Walking on the block                    block.wood.step	    0.15    1.0     16

	Block data:
		Standing:
			direction
		Wall:
			direction
*/


class cBlockBannerHandler :
	public cYawRotator<cBlockEntityHandler, 0x03, 0x02, 0x03, 0x00, 0x01>
{
	using Super = cYawRotator<cBlockEntityHandler, 0x03, 0x02, 0x03, 0x00, 0x01>;

  public:

	cBlockBannerHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	// Overrides:
	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		const Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta
	) override;

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos
	) override;

	virtual cItems ConvertToPickups(
		NIBBLETYPE a_BlockMeta,
		cBlockEntity * a_BlockEntity,
		const cEntity * a_Digger,
		const cItem * a_Tool
	) override;

	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player,
		const sSetBlock & a_BlockChange
	) override;





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 28;
	}
} ;




