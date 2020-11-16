
#pragma once

#include "BlockEntity.h"





class cBlockNoteBlockHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

private:

	virtual void OnDigging(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos
	) const override
	{
		a_WorldInterface.DoWithBlockEntityAt(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, [](cBlockEntity & a_Entity)
		{
			if (a_Entity.GetBlockType() != E_BLOCK_NOTE_BLOCK)
			{
				return false;
			}

			auto & NoteEntity = static_cast<cNoteEntity &>(a_Entity);

			NoteEntity.MakeSound();

			return true;
		});
	}
} ;




