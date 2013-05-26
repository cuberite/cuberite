
// DropperEntity.cpp

// Implements the cRtopperEntity class representing a Dropper block entity

#include "Globals.h"
#include "DropperEntity.h"
#include "Player.h"
#include "Simulator/FluidSimulator.h"





cDropperEntity::cDropperEntity(int a_BlockX, int a_BlockY, int a_BlockZ) :
	super(E_BLOCK_DROPPER, a_BlockX, a_BlockY, a_BlockZ, NULL)
{
	SetBlockEntity(this);  // cBlockEntityWindowOwner
}





cDropperEntity::cDropperEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_DROPPER, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
	SetBlockEntity(this);  // cBlockEntityWindowOwner
}





void cDropperEntity::DropSpenseFromSlot(int a_SlotNum)
{
	DropFromSlot(a_SlotNum);
}




