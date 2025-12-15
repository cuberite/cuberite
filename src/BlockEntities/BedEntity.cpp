
// BedEntity.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BedEntity.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../ClientHandle.h"
#include "../Blocks/BlockBed.h"
#include "../Registries/BlockItemConverter.h"





cBedEntity::cBedEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World, short a_Color):
	Super(a_Block, a_Pos, a_World),
	m_Color(a_Color)
{
	ASSERT(cBlockBedHandler::IsBlockBed(a_Block.Type()));
}





cItems cBedEntity::ConvertToPickups() const
{
	return cItem(BlockItemConverter::FromBlock(m_Block.Type()));
}





void cBedEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cBedEntity &>(a_Src);
	m_Color = src.m_Color;
}





void cBedEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





void cBedEntity::SetColor(short a_Color)
{
	m_Color = a_Color;
}



#define CHECK_BED_HEAD(BedType) \
	case BlockType::BedType: return Block::BedType::Part(m_Block) == Block::BedType::Part::Head;

bool cBedEntity::IsPillowBlock(void) const
{
	switch (m_Block.Type())
	{
		CHECK_BED_HEAD(WhiteBed)
		CHECK_BED_HEAD(LightGrayBed)
		CHECK_BED_HEAD(GrayBed)
		CHECK_BED_HEAD(BlackBed)
		CHECK_BED_HEAD(BrownBed)
		CHECK_BED_HEAD(RedBed)
		CHECK_BED_HEAD(OrangeBed)
		CHECK_BED_HEAD(YellowBed)
		CHECK_BED_HEAD(LimeBed)
		CHECK_BED_HEAD(GreenBed)
		CHECK_BED_HEAD(CyanBed)
		CHECK_BED_HEAD(LightBlueBed)
		CHECK_BED_HEAD(BlueBed)
		CHECK_BED_HEAD(PurpleBed)
		CHECK_BED_HEAD(MagentaBed)
		CHECK_BED_HEAD(PinkBed)
		default: UNREACHABLE("Invalid bed type");
	}
}
