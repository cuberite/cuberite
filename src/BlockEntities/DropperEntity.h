
// DropperEntity.h

// Declares the cDropperEntity class representing a dropper block entity





#pragma once

#include "DropSpenserEntity.h"





// tolua_begin
class cDropperEntity :
	public cDropSpenserEntity
{
	// tolua_end

	using super = cDropSpenserEntity;

public:  // tolua_export

	BLOCKENTITY_PROTODEF(cDropperEntity)

	/** Constructor used for normal operation */
	cDropperEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);


protected:

	// cDropSpenserEntity overrides:
	virtual void DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum) override;
} ;  // tolua_export




