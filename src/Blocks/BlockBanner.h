
// BlockBanner.h

#pragma once

#include "BlockEntity.h"
#include "Mixins.h"
#include "ChunkInterface.h"


class cEntity;
class cPlayer;
class cWorldInterface;




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




