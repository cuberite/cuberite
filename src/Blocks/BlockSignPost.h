
#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"





class cBlockSignPostHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	/** Converts the (player) rotation to placed-signpost block meta. */
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

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		return cItem(E_ITEM_SIGN, 1, 0);
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}
		BLOCKTYPE Type = a_Chunk.GetBlock(a_RelPos.addedY(-1));
		return ((Type == E_BLOCK_SIGN_POST) || (Type == E_BLOCK_WALLSIGN) || cBlockInfo::IsSolid(Type));
	}





	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) const override
	{
		return (a_Meta + 4) & 0x0f;
	}





	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) const override
	{
		return (a_Meta + 12) & 0x0f;
	}





	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) const override
	{
		// Mirrors signs over the XY plane (North-South Mirroring)

		// There are 16 meta values which correspond to different directions.
		// These values are equated to angles on a circle; 0x08 = 180 degrees.
		return (a_Meta < 0x08) ? (0x08 - a_Meta) : (0x18 - a_Meta);
	}





	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) const override
	{
		// Mirrors signs over the YZ plane (East-West Mirroring)

		// There are 16 meta values which correspond to different directions.
		// These values are equated to angles on a circle; 0x10 = 360 degrees.
		return 0x0f - a_Meta;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 13;
	}
} ;




