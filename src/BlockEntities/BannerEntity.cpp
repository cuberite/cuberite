
// BannerEntity.cpp

// Implements the cBannerEntity class representing a banner block in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "BannerEntity.h"

#include "../World.h"
#include "../ClientHandle.h"
#include "../Blocks/BlockBanner.h"





cBannerEntity::cBannerEntity(BlocState a_Block, Vector3i a_Pos, cWorld * a_World) :
	cBannerEntity(a_Block, a_Pos, a_World, 1)
{
}





cBannerEntity::cBannerEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World, unsigned char a_BaseColor):
	Super(a_Block, a_Pos, a_World),
	m_BaseColor(a_BaseColor)
{
	ASSERT(cBlockBannerHandler::IsBlockBanner(a_Block));
}





void cBannerEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cBannerEntity &>(a_Src);
	m_BaseColor = src.m_BaseColor;
}





void cBannerEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendBlockChange(m_Pos.x, m_Pos.y, m_Pos.z, m_Block);
	a_Client.SendUpdateBlockEntity(*this);
}





cItems cBannerEntity::ConvertToPickups() const
{
	return cItem(E_ITEM_BANNER, 1, static_cast<NIBBLETYPE>(GetBaseColor()));
}





unsigned char cBannerEntity::GetBaseColor() const
{
	return m_BaseColor;
}





void cBannerEntity::SetBaseColor(const unsigned char a_Color)
{
	m_BaseColor = a_Color;
}
