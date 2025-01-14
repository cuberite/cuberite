
// BannerEntity.cpp

// Implements the cBannerEntity class representing a banner block in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "BannerEntity.h"

#include "../World.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "../Registries/BlockItemConverter.h"
#include "../Blocks/BlockStandingBanner.h"





cBannerEntity::cBannerEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World) :
	cBannerEntity(a_Block, a_Pos, a_World, 1)
{
}





cBannerEntity::cBannerEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World, unsigned char a_BaseColor):
	Super(a_Block, a_Pos, a_World),
	m_BaseColor(a_BaseColor)
{
	ASSERT(cBlockStandingBannerHandler::IsBlockBanner(a_Block));
}





cItems cBannerEntity::ConvertToPickups() const
{
	return cItem(BlockItemConverter::FromBlock(m_Block.Type()));
}





void cBannerEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cBannerEntity &>(a_Src);
	m_BaseColor = src.m_BaseColor;
	m_CustomName = src.m_CustomName;
}





void cBannerEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendBlockChange(m_Pos, m_Block);
	a_Client.SendUpdateBlockEntity(*this);
}





bool cBannerEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
	return false;
}
