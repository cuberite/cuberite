
// DropperEntity.cpp

// Implements the cRtopperEntity class representing a Dropper block entity

#include "Globals.h"
#include "DropperEntity.h"





cDropperEntity::cDropperEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World)
{
	ASSERT(a_BlockType == E_BLOCK_DROPPER);
}





void cDropperEntity::DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum)
{
	DropFromSlot(a_Chunk, a_SlotNum);
}




