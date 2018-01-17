
// BlockBed.h

#pragma once

#include "BlockEntity.h"
#include "MetaRotator.h"
#include "ChunkInterface.h"


class cEntity;
class cPlayer;
class cWorldInterface;




class cBlockBedHandler :
	public cMetaRotator<cBlockEntityHandler, 0x3, 0x02, 0x03, 0x00, 0x01, true>
{
public:
	cBlockBedHandler(BLOCKTYPE a_BlockType)
		: cMetaRotator<cBlockEntityHandler, 0x3, 0x02, 0x03, 0x00, 0x01, true>(a_BlockType)
	{
	}

	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override;

	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;

	virtual bool IsUseable(void) override
	{
		return true;
	}

	virtual void ConvertToPickups(cItems & Pickups, NIBBLETYPE Meta) override {}

	virtual void ConvertToPickups(cWorldInterface & a_WorldInterface, cItems & a_Pickups, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ) override;

	// Bed specific helper functions
	static NIBBLETYPE RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 180 + (180 / 4);  // So its not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}

		a_Rotation = (a_Rotation / 360) * 4;

		return (static_cast<NIBBLETYPE>(a_Rotation + 2)) % 4;
	}

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

	static void SetBedOccupationState(cChunkInterface & a_ChunkInterface, const Vector3i & a_BedPosition, bool a_IsOccupied)
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

		a_ChunkInterface.SetBlockMeta(a_BedPosition.x, a_BedPosition.y, a_BedPosition.z, Meta);
	}

	virtual void OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, const sSetBlock & a_BlockChange) override;

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 28;
	}
} ;




