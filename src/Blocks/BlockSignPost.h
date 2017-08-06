
#pragma once

#include "BlockHandler.h"
#include "Chunk.h"





class cBlockSignPostHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;

public:
	cBlockSignPostHandler(BLOCKTYPE a_BlockType) :
		super(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SIGN, 1, 0));
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}
		BLOCKTYPE Type = a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ);

		return ((Type == E_BLOCK_SIGN_POST) || (Type == E_BLOCK_WALLSIGN) || cBlockInfo::IsSolid(Type));
	}

	static NIBBLETYPE RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 180 + (180 / 16);  // So it's not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}

		a_Rotation = (a_Rotation / 360) * 16;

		return (static_cast<char>(a_Rotation)) % 16;
	}

	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		return (a_Meta + 4) & 0x0f;
	}

	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		return (a_Meta + 12) & 0x0f;
	}

	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Mirrors signs over the XY plane (North-South Mirroring)

		// There are 16 meta values which correspond to different directions.
		// These values are equated to angles on a circle; 0x08 = 180 degrees.
		return (a_Meta < 0x08) ? (0x08 - a_Meta) : (0x18 - a_Meta);
	}

	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Mirrors signs over the YZ plane (East-West Mirroring)

		// There are 16 meta values which correspond to different directions.
		// These values are equated to angles on a circle; 0x10 = 360 degrees.
		return 0x0f - a_Meta;
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 13;
	}
} ;




