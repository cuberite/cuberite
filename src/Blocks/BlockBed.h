
// BlockBed.h

#pragma once

#include "BlockEntity.h"
#include "Mixins.h"
#include "ChunkInterface.h"


class cEntity;
class cPlayer;
class cWorldInterface;




class cBlockBedHandler :
	public cYawRotator<cBlockEntityHandler, 0x03, 0x02, 0x03, 0x00, 0x01>
{
	using Super = cYawRotator<cBlockEntityHandler, 0x03, 0x02, 0x03, 0x00, 0x01>;

public:

	cBlockBedHandler(BLOCKTYPE a_BlockType):
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





	static Vector3i MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0: return Vector3i( 0, 0,  1);
			case 1: return Vector3i(-1, 0,  0);
			case 2: return Vector3i( 0, 0, -1);
			case 3: return Vector3i( 1, 0,  0);
		}
		return Vector3i();
	}





	static void SetBedOccupationState(cChunkInterface & a_ChunkInterface, Vector3i a_BedPosition, bool a_IsOccupied)
	{
		auto Meta = a_ChunkInterface.GetBlockMeta(a_BedPosition);
		if (a_IsOccupied)
		{
			Meta |= 0x04;  // Where 0x4 = occupied bit
		}
		else
		{
			Meta &= 0x0b;  // Clear the "occupied" bit of the bed's block
		}

		a_ChunkInterface.SetBlockMeta(a_BedPosition, Meta);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 28;
	}
} ;




