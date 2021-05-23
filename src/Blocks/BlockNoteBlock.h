
#pragma once

#include "BlockEntity.h"
#include "../BlockEntities/NoteEntity.h"





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
		a_WorldInterface.DoWithBlockEntityAt(a_BlockPos, [](cBlockEntity & a_BlockEntity)
		{
			ASSERT(a_BlockEntity.GetBlockType() == BlockType::NoteBlock);

			static_cast<cNoteEntity &>(a_BlockEntity).MakeSound();
			return false;
		});
	}
};
