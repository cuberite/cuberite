
// BannerEntity.cpp

// Implements the cBannerEntity class representing a single command block in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "BannerEntity.h"

#include "../Root.h"
#include "../World.h"
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





bool cBannerEntity::HasPatterns() const
{
	return m_PatternContainer->HasPatterns();
}





const cBannerPatternContainer * cBannerEntity::GetPatternContainer() const
{
	return m_PatternContainer;
}





short cBannerEntity::GetBaseColor() const
{
	return m_BaseColor;
}


////////////////////////////////////////////////////////////////////////////////
// cBannerPattern


cBannerPattern::cBannerPattern() {}





cBannerPattern::cBannerPattern(BannerPattern a_Pattern, short a_Color)
{
	m_Pattern = a_Pattern;
	m_Color = a_Color;
}





const BannerPattern cBannerPattern::GetPattern() const
{
	return m_Pattern;
}





const short cBannerPattern::GetPatternColor() const
{
	return m_Color;
}





const char * cBannerPattern::GetPatternTag(BannerPattern a_Pattern)
{
	switch (a_Pattern)
	{
		case BannerPattern::BottomStripe: return "bs";
		case BannerPattern::TopStripe: return "ts";
		case BannerPattern::LeftStripe: return "ls";
		case BannerPattern::RightStripe: return "rs";
		case BannerPattern::CenterStripeVertical: return "cs";
		case BannerPattern::MiddleStripeHorizontal: return "ms";
		case BannerPattern::DownRightStripe: return "drs";
		case BannerPattern::DownLeftStripe: return "dls";
		case BannerPattern::SmallVerticalStripes: return "ss";
		case BannerPattern::DiagonalCross: return "cr";
		case BannerPattern::SquareCross: return "sc";
		case BannerPattern::LeftOfDiagonal: return "ld";
		case BannerPattern::RightOfUpsideDownDiagonal: return "rud";
		case BannerPattern::LeftOfUpsideDownDiagonal: return "lud";
		case BannerPattern::RightOfDiagonal: return "rd";
		case BannerPattern::VerticalHalfLeft: return "vh";
		case BannerPattern::VerticalHalfRight: return "vhr";
		case BannerPattern::HorizontalHalfTop: return "hh";
		case BannerPattern::HorizontalHalfBottom: return "hhb";
		case BannerPattern::BottomLeftCorner: return "bl";
		case BannerPattern::BottomRightCorner: return "br";
		case BannerPattern::TopLeftCorner: return "tl";
		case BannerPattern::TopRightCorner: return "tr";
		case BannerPattern::BottomTriangle: return "bt";
		case BannerPattern::TopTriangle: return "tt";
		case BannerPattern::BottomTriangleSawtooth: return "bts";
		case BannerPattern::TopTriangleSawtooth: return "tts";
		case BannerPattern::MiddleCircle: return "mc";
		case BannerPattern::MiddleRhombus: return "mr";
		case BannerPattern::Border: return "bo";
		case BannerPattern::CurlyBorder: return "cbo";
		case BannerPattern::Brick: return "bri";
		case BannerPattern::Gradient: return "gra";
		case BannerPattern::GradientUpsideDown: return "gru";
		case BannerPattern::Creeper: return "cre";
		case BannerPattern::Skull: return "sku";
		case BannerPattern::Flower: return "flo";
		case BannerPattern::Mojang: return "moj";
		case BannerPattern::Globe: return "glb";
		case BannerPattern::Piglin: return "pig";
		default: return "";
	}
}


////////////////////////////////////////////////////////////////////////////////
// cBannerPatternContainer


cBannerPatternContainer::cBannerPatternContainer() {}





bool cBannerPatternContainer::AddPattern(cBannerPattern a_Pattern, bool a_ByCommand)
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





const cBannerPattern * cBannerPatternContainer::GetPattern(short a_Pattern) const
{
	return & m_Patterns[a_Pattern];
}





void cBannerPatternContainer::ClearAll()
{
	// only clean the used ones
	for (short i = 0; i < m_PatternCount; i++)
	{
		m_Patterns[i] = cBannerPattern();
	}
}
