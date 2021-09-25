
// BlockBed.h

#pragma once

#include "BlockEntity.h"
#include "ChunkInterface.h"
#include "Entities/Player.h"
#include "Mixins.h"





class cEntity;
class cWorldInterface;





class cBlockBedHandler final :
	public cYawRotator<cBlockEntityHandler, 0x03, 0x02, 0x03, 0x00, 0x01>
{
	using Super = cYawRotator<cBlockEntityHandler, 0x03, 0x02, 0x03, 0x00, 0x01>;

public:

	using Super::Super;

	static Vector3i MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0: return Vector3i(0, 0, 1);
			case 1: return Vector3i(-1, 0, 0);
			case 2: return Vector3i(0, 0, -1);
			case 3: return Vector3i(1, 0, 0);
		}
		return Vector3i();
	}

	static void VacateBed(cChunkInterface & a_ChunkInterface, cPlayer & a_Player)
	{
		auto BedPosition = a_Player.GetLastBedPos();

		BLOCKTYPE Type;
		NIBBLETYPE Meta;
		a_ChunkInterface.GetBlockTypeMeta(BedPosition, Type, Meta);

		if (Type != E_BLOCK_BED)
		{
			// Bed was incomplete, just wake:
			a_Player.SetIsInBed(false);
			return;
		}

		if ((Meta & 0x8) == 0)
		{
			// BedPosition is the foot of the bed, adjust to the head:
			BedPosition += MetaDataToDirection(Meta & 0x03);

			a_ChunkInterface.GetBlockTypeMeta(BedPosition, Type, Meta);
			if (Type != E_BLOCK_BED)
			{
				// Bed was incomplete, just wake:
				a_Player.SetIsInBed(false);
				return;
			}
		}

		// Clear the "occupied" bit of the bed's pillow block:
		a_ChunkInterface.SetBlockMeta(BedPosition, Meta & 0x0b);

		// Wake the player:
		a_Player.SetIsInBed(false);
	}

private:

	// Overrides:
	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		const Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
		const cEntity * a_Digger
	) const override;

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos
	) const override;

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cItem * a_Tool) const override;





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 28;
	}
} ;
