
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

class cBannerEntity :
	public cBlockEntity
{
	// tolua_end

	using Super = cBlockEntity;

public:  // tolua_export

	/** Creates a new empty Banner entity */
	cBannerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override { return true; }
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual cItems ConvertToPickups() const override;

	// Functions for sending with network and storing to disk
	void SetBaseColor(unsigned char a_Color);
	unsigned char GetBaseColor() const;

	static const char * GetPatternTag(eBannerPattern a_Pattern);


protected:

	unsigned char m_BaseColor = 1;


} ;  // tolua_export
