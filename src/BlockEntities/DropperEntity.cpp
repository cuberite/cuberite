
// DropperEntity.cpp

// Implements the cRtopperEntity class representing a Dropper block entity

#include "Globals.h"
#include "DropperEntity.h"





cDropperEntity::cDropperEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_DROPPER, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
}





void cDropperEntity::DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum)
{
	DropFromSlot(a_Chunk, a_SlotNum);
}




