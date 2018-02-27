#pragma once

#include "BlockHandler.h"
#include "../FastRandom.h"





class cBlockCocoaPodHandler :
	public cBlockHandler
{
public:
	cBlockCocoaPodHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		eBlockFace BlockFace = MetaToBlockFace(a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ));
		AddFaceDirection(a_RelX, a_RelY, a_RelZ, BlockFace, true);

		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		a_Chunk.UnboundedRelGetBlock(a_RelX, a_RelY, a_RelZ, BlockType, BlockMeta);

		return ((BlockType == E_BLOCK_LOG) && ((BlockMeta & 0x3) == E_META_LOG_JUNGLE));
	}

	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		if (GetRandomProvider().RandBool(0.20))
		{
			NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
			NIBBLETYPE TypeMeta = Meta & 0x03;
			int GrowState = Meta >> 2;

			if (GrowState < 2)
			{
				++GrowState;
				a_Chunk.SetMeta(a_RelX, a_RelY, a_RelZ, static_cast<NIBBLETYPE>(GrowState << 2 | TypeMeta));
			}
		}
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		int GrowState = a_BlockMeta >> 2;
		a_Pickups.Add(E_ITEM_DYE, ((GrowState >= 2) ? 3 : 1), E_META_DYE_BROWN);
	}

	static eBlockFace MetaToBlockFace(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x3)
		{
			case 0: return BLOCK_FACE_ZM;
			case 1: return BLOCK_FACE_XP;
			case 2: return BLOCK_FACE_ZP;
			case 3: return BLOCK_FACE_XM;
			default:
			{
				ASSERT(!"Bad meta");
				return BLOCK_FACE_NONE;
			}
		}
	}

	static NIBBLETYPE BlockFaceToMeta(eBlockFace a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_ZM: return 0;
			case BLOCK_FACE_XM: return 3;
			case BLOCK_FACE_XP: return 1;
			case BLOCK_FACE_ZP: return 2;
			case BLOCK_FACE_NONE:
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				ASSERT(!"Unknown face");
				return 0;
			}
		}
		UNREACHABLE("Unsupported block face");
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 34;
	}
} ;




