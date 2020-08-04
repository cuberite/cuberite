
// BannerEntity.cpp

// Implements the cBannerEntity class representing a single command block in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "BannerEntity.h"

#include "../Root.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../BlockType.h"
#include "../WorldStorage/FastNBT.h"
#include "../ClientHandle.h"


////////////////////////////////////////////////////////////////////////////////
// cBannerEntity


cBannerEntity::cBannerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World)
{
	ASSERT((a_BlockType == E_BLOCK_WALL_BANNER) || (a_BlockType == E_BLOCK_STANDING_BANNER));
	m_PatternContainer = new cBannerPatternContainer();
}





cBannerEntity::~cBannerEntity()
{
	delete [] m_PatternContainer;
}





void cBannerEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cBannerEntity &>(a_Src);
	m_BaseColor = src.m_BaseColor;
	m_PatternContainer = new cBannerPatternContainer();
	m_PatternContainer->m_PatternCount = src.m_PatternContainer->m_PatternCount;
	for (auto i = 0; i < m_PatternContainer->GetPatternCount(); i++)
	{
		m_PatternContainer->m_Patterns[i] = src.m_PatternContainer->m_Patterns[i];
	}
	// Todo: properly copy
}





void cBannerEntity::SendTo(cClientHandle & a_Client)
{
	cWorld * World = a_Client.GetPlayer()->GetWorld();
	a_Client.SendBlockChange(m_Pos.x, m_Pos.y, m_Pos.z, m_BlockType, World->GetBlockMeta(GetPos()));
	a_Client.SendUpdateBlockEntity(*this);
}





void cBannerEntity::SetBaseColor(unsigned char a_Color)
{
	m_BaseColor = a_Color;
}





void cBannerEntity::SetId(AString * a_Id)
{
	m_Id = std::move(a_Id);
}





bool cBannerEntity::HasPatterns() const
{
	return m_PatternContainer->HasPatterns();
}





const cBannerPatternContainer * cBannerEntity::GetPatternContainer() const
{
	return m_PatternContainer;
}





unsigned char cBannerEntity::GetBaseColor() const
{
	return m_BaseColor;
}


////////////////////////////////////////////////////////////////////////////////
// cBannerPatternContainer


cBannerPatternContainer::cBannerPatternContainer() {}





bool cBannerPatternContainer::AddPattern(BannerPattern & a_Pattern, bool a_ByCommand)
{
	// check if there is space for another pattern - with crating max is 6 and by command 16
	if (((a_ByCommand) && (m_PatternCount == 16)) || ((!a_ByCommand) && (m_PatternCount == 6)))
	{
		return false;
	}
	m_Patterns[m_PatternCount - 1] = a_Pattern;
	m_PatternCount++;
	return true;
}





bool cBannerPatternContainer::HasPatterns() const
{
	return m_PatternCount > 0;
}





const short cBannerPatternContainer::GetPatternCount() const
{
	return m_PatternCount;
}





const BannerPattern * cBannerPatternContainer::GetPattern(short a_Pattern) const
{
	return & m_Patterns[a_Pattern];
}





void cBannerPatternContainer::ClearAll()
{
	// only clean the used ones
	for (short i = 0; i < m_PatternCount; i++)
	{
		m_Patterns[i] = {};
	}
}





const char * cBannerPatternContainer::GetPatternTag(eBannerPattern a_Pattern)
{
	switch (a_Pattern)
	{
		case eBannerPattern::BottomStripe: return "bs";
		case eBannerPattern::TopStripe: return "ts";
		case eBannerPattern::LeftStripe: return "ls";
		case eBannerPattern::RightStripe: return "rs";
		case eBannerPattern::CenterStripeVertical: return "cs";
		case eBannerPattern::MiddleStripeHorizontal: return "ms";
		case eBannerPattern::DownRightStripe: return "drs";
		case eBannerPattern::DownLeftStripe: return "dls";
		case eBannerPattern::SmallVerticalStripes: return "ss";
		case eBannerPattern::DiagonalCross: return "cr";
		case eBannerPattern::SquareCross: return "sc";
		case eBannerPattern::LeftOfDiagonal: return "ld";
		case eBannerPattern::RightOfUpsideDownDiagonal: return "rud";
		case eBannerPattern::LeftOfUpsideDownDiagonal: return "lud";
		case eBannerPattern::RightOfDiagonal: return "rd";
		case eBannerPattern::VerticalHalfLeft: return "vh";
		case eBannerPattern::VerticalHalfRight: return "vhr";
		case eBannerPattern::HorizontalHalfTop: return "hh";
		case eBannerPattern::HorizontalHalfBottom: return "hhb";
		case eBannerPattern::BottomLeftCorner: return "bl";
		case eBannerPattern::BottomRightCorner: return "br";
		case eBannerPattern::TopLeftCorner: return "tl";
		case eBannerPattern::TopRightCorner: return "tr";
		case eBannerPattern::BottomTriangle: return "bt";
		case eBannerPattern::TopTriangle: return "tt";
		case eBannerPattern::BottomTriangleSawtooth: return "bts";
		case eBannerPattern::TopTriangleSawtooth: return "tts";
		case eBannerPattern::MiddleCircle: return "mc";
		case eBannerPattern::MiddleRhombus: return "mr";
		case eBannerPattern::Border: return "bo";
		case eBannerPattern::CurlyBorder: return "cbo";
		case eBannerPattern::Brick: return "bri";
		case eBannerPattern::Gradient: return "gra";
		case eBannerPattern::GradientUpsideDown: return "gru";
		case eBannerPattern::Creeper: return "cre";
		case eBannerPattern::Skull: return "sku";
		case eBannerPattern::Flower: return "flo";
		case eBannerPattern::Mojang: return "moj";
		case eBannerPattern::Globe: return "glb";
		case eBannerPattern::Piglin: return "pig";
		default: return "";
	}
}
