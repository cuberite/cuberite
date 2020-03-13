#pragma once

#include "BlockHandler.h"
#include "../FastRandom.h"





class cBlockCocoaPodHandler :
	public cBlockHandler
{
	using super = cBlockHandler;

public:
	cBlockCocoaPodHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
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
			Grow(a_Chunk, {a_RelX, a_RelY, a_RelZ});
		}
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// If fully grown, give 3 items, otherwise just one:
		auto growState = a_BlockMeta >> 2;
		return cItem(E_ITEM_DYE, ((growState >= 2) ? 3 : 1), E_META_DYE_BROWN);
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) override
	{
		auto meta = a_Chunk.GetMeta(a_RelPos);
		auto typeMeta = meta & 0x03;
		auto growState = meta >> 2;

		if (growState >= 3)
		{
			return 0;
		}
		auto newState = std::min(growState + a_NumStages, 3);
		a_Chunk.SetMeta(a_RelPos, static_cast<NIBBLETYPE>(newState << 2 | typeMeta));
		return newState - growState;
	}





	static eBlockFace MetaToBlockFace(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x03)
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




