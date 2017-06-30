
// BedEntity.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BedEntity.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../ClientHandle.h"
#include "../Blocks/BlockBed.h"

cBedEntity::cBedEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, a_World),
	m_Color(0)
{
	ASSERT(a_BlockType == E_BLOCK_BED);
}





void cBedEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = reinterpret_cast<const cBedEntity &>(a_Src);
	m_Color = src.m_Color;
}





void cBedEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}
