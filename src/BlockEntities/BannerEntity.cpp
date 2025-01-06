
// BannerEntity.cpp

// Implements the cBannerEntity class representing a banner block in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "BannerEntity.h"

#include "../World.h"
#include "../ClientHandle.h"





cBannerEntity::cBannerEntity(BLOCKTYPE a_BlockType, BLOCKMETATYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World, unsigned char a_BaseColor, AString a_CustomName):
	Super(a_BlockType, static_cast<BLOCKMETATYPE>(a_BaseColor << 4) | a_BlockMeta, a_Pos, a_World),
	m_CustomName(std::move(a_CustomName))
{
	ASSERT((a_BlockType == E_BLOCK_WALL_BANNER) || (a_BlockType == E_BLOCK_STANDING_BANNER));
}





cItems cBannerEntity::ConvertToPickups() const
{
	cItem Item(E_ITEM_BANNER, 1, static_cast<NIBBLETYPE>(GetBaseColor()));
	Item.m_CustomName = m_CustomName;
	return Item;
}





void cBannerEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cBannerEntity &>(a_Src);
	m_CustomName = src.m_CustomName;
}





void cBannerEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendBlockChange(m_Pos, m_BlockType, m_BlockMeta);
	a_Client.SendUpdateBlockEntity(*this);
}





bool cBannerEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
	return false;
}
