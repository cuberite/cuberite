
#pragma once

#include "BlockHandler.h"
#include "ClearMetaOnDrop.h"





class cBlockLadderHandler :
	public cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x02, 0x05, 0x03, 0x04> >
{
	typedef cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x02, 0x05, 0x03, 0x04> > super;
public:
	cBlockLadderHandler(BLOCKTYPE a_BlockType)
		: super(a_BlockType)
	{
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (!LadderCanBePlacedAt(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace))
		{
			a_BlockFace = FindSuitableBlockFace(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);

			if (a_BlockFace == BLOCK_FACE_BOTTOM)
			{
				return false;
			}
		}

		a_BlockType = m_BlockType;
		a_BlockMeta = DirectionToMetaData(a_BlockFace);
		return true;
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.Add(m_BlockType, 1, 0);  // Reset meta
	}

	static NIBBLETYPE DirectionToMetaData(eBlockFace a_Direction)
	{
		switch (a_Direction)
		{
			case BLOCK_FACE_ZM: return 0x2;
			case BLOCK_FACE_ZP: return 0x3;
			case BLOCK_FACE_XM: return 0x4;
			case BLOCK_FACE_XP: return 0x5;
			case BLOCK_FACE_NONE:
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return 0x2;
			}
		}
		UNREACHABLE("Unsupported block face");
	}

	static eBlockFace MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0x2: return BLOCK_FACE_ZM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_XM;
			case 0x5: return BLOCK_FACE_XP;
			default:  return BLOCK_FACE_ZM;
		}
	}

	/** Finds a suitable Direction for the Ladder. Returns BLOCK_FACE_BOTTOM on failure */
	static eBlockFace FindSuitableBlockFace(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		for (int FaceInt = BLOCK_FACE_ZM; FaceInt <= BLOCK_FACE_XP; FaceInt++)
		{
			eBlockFace Face = static_cast<eBlockFace>(FaceInt);
			if (LadderCanBePlacedAt(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ, Face))
			{
				return Face;
			}
		}
		return BLOCK_FACE_BOTTOM;
	}

	static bool LadderCanBePlacedAt(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace)
	{
		if ((a_BlockFace == BLOCK_FACE_BOTTOM) || (a_BlockFace == BLOCK_FACE_TOP))
		{
			return false;
		}

		AddFaceDirection( a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);

		return cBlockInfo::IsSolid(a_ChunkInterface.GetBlock({a_BlockX, a_BlockY, a_BlockZ}));
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		// TODO: Use AdjustCoordsByMeta(), then cChunk::UnboundedRelGetBlock() and finally some comparison
		eBlockFace BlockFace = MetaDataToDirection(a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ));
		int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
		return LadderCanBePlacedAt(a_ChunkInterface, BlockX, a_RelY, BlockZ, BlockFace);
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




