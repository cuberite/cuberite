
// BannerEntity.h

// Declares the cBannerEntity class representing a single banner in the world





#pragma once

#include "BlockEntity.h"
#include "../Defines.h"


enum class BannerPattern
{
	BottomStripe,
	TopStripe,
	LeftStripe,
	RightStripe,
	CenterStripeVertical,
	MiddleStripeHorizontal,
	DownRightStripe,
	DownLeftStripe,
	SmallVerticalStripes,
	DiagonalCross,
	SquareCross,
	LeftOfDiagonal,
	RightOfUpsideDownDiagonal,
	LeftOfUpsideDownDiagonal,
	RightOfDiagonal,
	VerticalHalfLeft,
	VerticalHalfRight,
	HorizontalHalfTop,
	HorizontalHalfBottom,
	BottomLeftCorner,
	BottomRightCorner,
	TopLeftCorner,
	TopRightCorner,
	BottomTriangle,
	TopTriangle,
	BottomTriangleSawtooth,
	TopTriangleSawtooth,
	MiddleCircle,
	MiddleRhombus,
	Border,
	CurlyBorder,
	Brick,
	Gradient,
	GradientUpsideDown,
	Creeper,
	Skull,
	Flower,
	Mojang,
	Globe,
	Piglin
};

class cBannerPatternContainer;
class cBannerPattern;

// tolua_begin

class cBannerEntity :
	public cBlockEntity
{
	// tolua_end

	using Super = cBlockEntity;

public:  // tolua_export

	BLOCKENTITY_PROTODEF(cBannerEntity)

	/** Creates a new empty Banner entity */
	cBannerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);
	~cBannerEntity();

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override { return false; }
	virtual void SendTo(cClientHandle & a_Client) override;

	// Functions for sending with network
	bool HasPatterns() const;
	short GetBaseColor() const;
	const cBannerPatternContainer * GetPatternContainer() const;

protected:

	short m_BaseColor;
	cBannerPatternContainer * m_PatternContainer;

} ;  // tolua_export




/** class representing a Pattern */
class cBannerPattern
{
public:
	cBannerPattern();
	cBannerPattern(BannerPattern a_Pattern, short a_Color);

	const BannerPattern GetPattern() const;
	const short GetPatternColor() const;

	static const char * GetPatternTag(BannerPattern a_Pattern);

private:
	BannerPattern m_Pattern;
	short m_Color;
};




/** class to store the patterns contained in a banner */
class cBannerPatternContainer
{

public:
	cBannerPatternContainer();

	bool AddPattern(cBannerPattern a_Pattern, bool a_ByCommand);
	bool HasPatterns() const;
	const short GetPatternCount() const;
	const cBannerPattern * GetPattern(short a_Pattern) const;

	void ClearAll();

protected:
	cBannerPattern m_Patterns[16];
	short m_PatternCount = 0;
};
