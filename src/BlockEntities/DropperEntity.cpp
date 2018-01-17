
// DropperEntity.cpp

// Implements the cRtopperEntity class representing a Dropper block entity

#include "Globals.h"
#include "DropperEntity.h"





cDropperEntity::cDropperEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
	ASSERT(a_BlockType == E_BLOCK_DROPPER);
}





void cDropperEntity::DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum)
{
	DropFromSlot(a_Chunk, a_SlotNum);
}




