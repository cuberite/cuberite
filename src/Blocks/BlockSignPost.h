
#pragma once

#include "BlockHandler.h"
#include "BlockWallSign.h"
#include "../Chunk.h"





class cBlockSignPostHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockSignPost(BlockState a_Block)
	{
		// TODO: implement wall signs and hanging variants
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:
			case BlockType::BirchSign:
			case BlockType::CrimsonSign:
			case BlockType::DarkOakSign:
			case BlockType::JungleSign:
			case BlockType::OakSign:
			case BlockType::SpruceSign:
			case BlockType::WarpedSign:
			case BlockType::CherrySign:
			case BlockType::BambooSign:
			case BlockType::MangroveSign:
			case BlockType::PaleOakSign:
				return true;
			default: return false;
		}
	}

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(BlockItemConverter::FromBlock(m_BlockType));
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		const auto BelowPos = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(BelowPos))
		{
			return false;
		}
		auto Below = a_Chunk.GetBlock(a_Position.addedY(-1));
		return (IsBlockSignPost(Below) || cBlockWallSignHandler::IsBlockWallSign(Below) ||cBlockInfo::IsSolid(Below));
	}



#define ROTATE_CW_SIGN(SignType) \
	return SignType::SignType(  (SignType::Rotation(a_Block)  + 4) % 16, SignType::Waterlogged(a_Block)); \

	virtual BlockState RotateCW(BlockState a_Block) const override
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(  (AcaciaSign::Rotation(a_Block)  + 4) % 16, AcaciaSign::Waterlogged(a_Block));
			case BlockType::BirchSign:   return BirchSign::BirchSign(    (BirchSign::Rotation(a_Block)   + 4) % 16, BirchSign::Waterlogged(a_Block));
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign((CrimsonSign::Rotation(a_Block) + 4) % 16, CrimsonSign::Waterlogged(a_Block));
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign((DarkOakSign::Rotation(a_Block) + 4) % 16, DarkOakSign::Waterlogged(a_Block));
			case BlockType::JungleSign:  return JungleSign::JungleSign(  (JungleSign::Rotation(a_Block)  + 4) % 16, JungleSign::Waterlogged(a_Block));
			case BlockType::OakSign:     return OakSign::OakSign(        (OakSign::Rotation(a_Block)     + 4) % 16, OakSign::Waterlogged(a_Block));
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(  (SpruceSign::Rotation(a_Block)  + 4) % 16, SpruceSign::Waterlogged(a_Block));
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(  (WarpedSign::Rotation(a_Block)  + 4) % 16, WarpedSign::Waterlogged(a_Block));
			case BlockType::CherrySign:  ROTATE_CW_SIGN(CherrySign)
			case BlockType::BambooSign:  ROTATE_CW_SIGN(BambooSign)
			case BlockType::MangroveSign:  ROTATE_CW_SIGN(MangroveSign)
			case BlockType::PaleOakSign: ROTATE_CW_SIGN(PaleOakSign)
			default: return a_Block;
		}
	}



#define ROTATE_CCW_SIGN(SignType) \
	return SignType::SignType(  (SignType::Rotation(a_Block)  + 12) % 16, SignType::Waterlogged(a_Block)); \

	virtual BlockState RotateCCW(BlockState a_Block) const override
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(  (AcaciaSign::Rotation(a_Block)  + 12) % 16, AcaciaSign::Waterlogged(a_Block));
			case BlockType::BirchSign:   return BirchSign::BirchSign(    (BirchSign::Rotation(a_Block)   + 12) % 16, BirchSign::Waterlogged(a_Block));
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign((CrimsonSign::Rotation(a_Block) + 12) % 16, CrimsonSign::Waterlogged(a_Block));
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign((DarkOakSign::Rotation(a_Block) + 12) % 16, DarkOakSign::Waterlogged(a_Block));
			case BlockType::JungleSign:  return JungleSign::JungleSign(   (JungleSign::Rotation(a_Block) + 12) % 16, JungleSign::Waterlogged(a_Block));
			case BlockType::OakSign:     return OakSign::OakSign(         (OakSign::Rotation(a_Block)    + 12) % 16, OakSign::Waterlogged(a_Block));
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(   (SpruceSign::Rotation(a_Block) + 12) % 16, SpruceSign::Waterlogged(a_Block));
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(   (WarpedSign::Rotation(a_Block) + 12) % 16, WarpedSign::Waterlogged(a_Block));
			case BlockType::CherrySign:  ROTATE_CCW_SIGN(CherrySign)
			case BlockType::BambooSign:  ROTATE_CCW_SIGN(BambooSign)
			case BlockType::MangroveSign:  ROTATE_CCW_SIGN(MangroveSign)
			case BlockType::PaleOakSign: ROTATE_CCW_SIGN(PaleOakSign)
			default: return a_Block;
		}
	}

	// TODO: check is mirrorXY and mirrorYZ are identical

#define MIRROR_XY_SIGN(SignType) \
	return SignType::SignType(  (SignType::Rotation(a_Block)  + 8) % 16, SignType::Waterlogged(a_Block)); \


	virtual BlockState MirrorXY(BlockState a_Block) const override
	{
		// Mirrors signs over the XY plane (North-South Mirroring)
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(  (AcaciaSign::Rotation(a_Block)  + 8) % 16, AcaciaSign::Waterlogged(a_Block));
			case BlockType::BirchSign:   return BirchSign::BirchSign(    (BirchSign::Rotation(a_Block)   + 8) % 16, BirchSign::Waterlogged(a_Block));
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign((CrimsonSign::Rotation(a_Block) + 8) % 16, CrimsonSign::Waterlogged(a_Block));
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign((DarkOakSign::Rotation(a_Block) + 8) % 16, DarkOakSign::Waterlogged(a_Block));
			case BlockType::JungleSign:  return JungleSign::JungleSign(   (JungleSign::Rotation(a_Block) + 8) % 16, JungleSign::Waterlogged(a_Block));
			case BlockType::OakSign:     return OakSign::OakSign(         (OakSign::Rotation(a_Block)    + 8) % 16, OakSign::Waterlogged(a_Block));
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(   (SpruceSign::Rotation(a_Block) + 8) % 16, SpruceSign::Waterlogged(a_Block));
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(   (WarpedSign::Rotation(a_Block) + 8) % 16, WarpedSign::Waterlogged(a_Block));
			case BlockType::CherrySign:  MIRROR_XY_SIGN(CherrySign)
			case BlockType::BambooSign:  MIRROR_XY_SIGN(BambooSign)
			case BlockType::MangroveSign:  MIRROR_XY_SIGN(MangroveSign)
			case BlockType::PaleOakSign: MIRROR_XY_SIGN(PaleOakSign)
			default: return a_Block;
		}
	}

#define MIRROR_YZ_SIGN(SignType) \
	return SignType::SignType(  (SignType::Rotation(a_Block)  + 8) % 16, SignType::Waterlogged(a_Block)); \


	// TODO: Sanity Check
	virtual BlockState MirrorYZ(BlockState a_Block) const override
	{
		// Mirrors signs over the YZ plane (East-West Mirroring)
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(  (AcaciaSign::Rotation(a_Block)  + 8) % 16, AcaciaSign::Waterlogged(a_Block));
			case BlockType::BirchSign:   return BirchSign::BirchSign(    (BirchSign::Rotation(a_Block)   + 8) % 16, BirchSign::Waterlogged(a_Block));
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign((CrimsonSign::Rotation(a_Block) + 8) % 16, CrimsonSign::Waterlogged(a_Block));
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign((DarkOakSign::Rotation(a_Block) + 8) % 16, DarkOakSign::Waterlogged(a_Block));
			case BlockType::JungleSign:  return JungleSign::JungleSign(   (JungleSign::Rotation(a_Block) + 8) % 16, JungleSign::Waterlogged(a_Block));
			case BlockType::OakSign:     return OakSign::OakSign(         (OakSign::Rotation(a_Block)    + 8) % 16, OakSign::Waterlogged(a_Block));
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(   (SpruceSign::Rotation(a_Block) + 8) % 16, SpruceSign::Waterlogged(a_Block));
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(   (WarpedSign::Rotation(a_Block) + 8) % 16, WarpedSign::Waterlogged(a_Block));
			case BlockType::CherrySign:  MIRROR_YZ_SIGN(CherrySign)
			case BlockType::BambooSign:  MIRROR_YZ_SIGN(BambooSign)
			case BlockType::MangroveSign:  MIRROR_YZ_SIGN(MangroveSign)
			case BlockType::PaleOakSign: MIRROR_YZ_SIGN(PaleOakSign)
			default: return a_Block;
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 13;
	}
} ;




