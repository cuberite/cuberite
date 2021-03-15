
// BannerEntity.cpp

// Implements the cBannerEntity class representing a banner block in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "BannerEntity.h"

#include "../World.h"
#include "../ClientHandle.h"





cBannerEntity::cBannerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World) :
	cBannerEntity(a_BlockType, a_BlockMeta, a_Pos, a_World, 1)
{
}





cBannerEntity::cBannerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World, unsigned char a_BaseColor):
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World),
	m_BaseColor(a_BaseColor)
{
	ASSERT((a_BlockType == E_BLOCK_WALL_BANNER) || (a_BlockType == E_BLOCK_STANDING_BANNER));
}





unsigned char cBannerEntity::GetBaseColor() const
{
	return m_BaseColor;
}





void cBannerEntity::SetBaseColor(const unsigned char a_Color)
{
	m_BaseColor = a_Color;
}





cItems cBannerEntity::ConvertToPickups() const
{
	return cItem(E_ITEM_BANNER, 1, static_cast<NIBBLETYPE>(GetBaseColor()));
}





void cBannerEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cBannerEntity &>(a_Src);
	m_BaseColor = src.m_BaseColor;
}





void cBannerEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendBlockChange(m_Pos.x, m_Pos.y, m_Pos.z, m_BlockType, m_BlockMeta);
	a_Client.SendUpdateBlockEntity(*this);
}





bool cBannerEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
	return false;
}
