
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"


class cBlockGlazedTerracottaHandler:
	public cClearMetaOnDrop<cBlockHandler>
{
	using super = cClearMetaOnDrop<cBlockHandler>;

public:

	cBlockGlazedTerracottaHandler(BLOCKTYPE a_BlockType) : super(a_BlockType)
	{
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;

		// FIXME: Do not use cPiston class for furnace placement!
		a_BlockMeta = cBlockPistonHandler::RotationPitchToMetaData(a_Player.GetYaw(), 0);

		return true;
	}

};
