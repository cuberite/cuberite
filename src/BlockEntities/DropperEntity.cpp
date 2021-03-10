
// DropperEntity.cpp

// Implements the cRtopperEntity class representing a Dropper block entity

#include "Globals.h"
#include "DropperEntity.h"





cDropperEntity::cDropperEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, a_World)
{
	ASSERT(a_Block.Type() == BlockType::Dropper);
}





void cDropperEntity::DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum)
{
	DropFromSlot(a_Chunk, a_SlotNum);
}




