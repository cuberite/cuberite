
// BedEntity.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BedEntity.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../ClientHandle.h"
#include "../Blocks/BlockBed.h"





cBedEntity::cBedEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World, short a_Color):
	Super(a_Block, a_Pos, a_World),
	m_Color(a_Color)
{
	ASSERT(IsBed(a_Block.Type()));
}





cItems cBedEntity::ConvertToPickups() const
{
	return cItem(E_ITEM_BED, 1, m_Color);
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





bool cBedEntity::IsPillowBlock(void)
{
	return
	(
		(Block::BlackBed::Part(m_Block)     == Block::BlackBed::Part::Head) ||
		(Block::BlueBed::Part(m_Block)      == Block::BlueBed::Part::Head) ||
		(Block::BrownBed::Part(m_Block)     == Block::BrownBed::Part::Head) ||
		(Block::CyanBed::Part(m_Block)      == Block::CyanBed::Part::Head) ||
		(Block::GrayBed::Part(m_Block)      == Block::GrayBed::Part::Head) ||
		(Block::GreenBed::Part(m_Block)     == Block::GreenBed::Part::Head) ||
		(Block::LightBlueBed::Part(m_Block) == Block::LightBlueBed::Part::Head) ||
		(Block::LightGrayBed::Part(m_Block) == Block::LightGrayBed::Part::Head) ||
		(Block::LimeBed::Part(m_Block)      == Block::LimeBed::Part::Head) ||
		(Block::MagentaBed::Part(m_Block)   == Block::MagentaBed::Part::Head) ||
		(Block::OrangeBed::Part(m_Block)    == Block::OrangeBed::Part::Head) ||
		(Block::PinkBed::Part(m_Block)      == Block::PinkBed::Part::Head) ||
		(Block::PurpleBed::Part(m_Block)    == Block::PurpleBed::Part::Head) ||
		(Block::RedBed::Part(m_Block)       == Block::RedBed::Part::Head) ||
		(Block::WhiteBed::Part(m_Block)     == Block::WhiteBed::Part::Head) ||
		(Block::YellowBed::Part(m_Block)    == Block::YellowBed::Part::Head)
	);
}





void cBedEntity::SetColor(short a_Color)
{
	m_Color = a_Color;
	auto Pos = GetPos();

	// If the bed entity is send immediately, the client (maybe) still has not the bed.
	// Fix that by delaying the broadcast of the bed entity by a tick:
	m_World->ScheduleTask(1, [Pos](cWorld & a_World)
	{
		a_World.BroadcastBlockEntity(Pos);
	});
}
