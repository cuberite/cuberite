
// BannerEntity.h

// Declares the cBannerEntity class representing a single banner in the world





#pragma once

#include "Globals.h"

#include "BlockEntity.h"
#include "../Defines.h"

// tolua_begin

enum class eBannerPattern
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
	Piglin,
	None
};




/** struct representing a Pattern */
struct BannerPattern
{
	eBannerPattern m_Pattern;
	short m_Color;
};





class cBannerEntity :
	public cBlockEntity
{
	// tolua_end

	using Super = cBlockEntity;

public:  // tolua_export

	BLOCKENTITY_PROTODEF(cBannerEntity)

	/** Creates a new empty Banner entity */
	cBannerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override { return true; }
	virtual void SendTo(cClientHandle & a_Client) override;

	// Functions for sending with network and storing to disk
	bool HasPatterns() const;
	void SetBaseColor(short a_Color);
	unsigned char GetBaseColor() const;

	// Pattern handling
	bool AddPattern(BannerPattern a_Pattern, bool a_ByCommand = false);
	const short GetPatternCount() const;
	const BannerPattern * GetPattern(short a_Pattern) const;
	void ClearAll();

	static const char * GetPatternTag(eBannerPattern a_Pattern);


protected:

	BannerPattern m_Patterns[16];
	short m_PatternCount = 0;

	short m_BaseColor = 1;


} ;  // tolua_export