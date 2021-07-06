
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
		if (a_Position.y <= 0)
		{
			return false;
		}
		auto Below = a_Chunk.GetBlock(a_Position.addedY(-1));
		return (IsBlockSignPost(Below) || cBlockWallSignHandler::IsBlockWallSign(Below) ||cBlockInfo::IsSolid(Below));
	}





	virtual BlockState RotateCW(BlockState a_Block) const override
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(  (AcaciaSign::Rotation(a_Block)  + 4) % 16);
			case BlockType::BirchSign:   return BirchSign::BirchSign(    (BirchSign::Rotation(a_Block)   + 4) % 16);
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign((CrimsonSign::Rotation(a_Block) + 4) % 16);
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign((DarkOakSign::Rotation(a_Block) + 4) % 16);
			case BlockType::JungleSign:  return JungleSign::JungleSign(  (JungleSign::Rotation(a_Block)  + 4) % 16);
			case BlockType::OakSign:     return OakSign::OakSign(        (OakSign::Rotation(a_Block)     + 4) % 16);
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(  (SpruceSign::Rotation(a_Block)  + 4) % 16);
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(  (WarpedSign::Rotation(a_Block)  + 4) % 16);
			default: return a_Block;
		}
	}





	virtual BlockState RotateCCW(BlockState a_Block) const override
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(  (AcaciaSign::Rotation(a_Block)  + 12) % 16);
			case BlockType::BirchSign:   return BirchSign::BirchSign(    (BirchSign::Rotation(a_Block)   + 12) % 16);
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign((CrimsonSign::Rotation(a_Block) + 12) % 16);
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign((DarkOakSign::Rotation(a_Block) + 12) % 16);
			case BlockType::JungleSign:  return JungleSign::JungleSign(   (JungleSign::Rotation(a_Block) + 12) % 16);
			case BlockType::OakSign:     return OakSign::OakSign(         (OakSign::Rotation(a_Block)    + 12) % 16);
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(   (SpruceSign::Rotation(a_Block) + 12) % 16);
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(   (WarpedSign::Rotation(a_Block) + 12) % 16);
			default: return a_Block;
		}
	}





	virtual BlockState MirrorXY(BlockState a_Block) const override
	{
		// Mirrors signs over the XY plane (North-South Mirroring)
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(  (AcaciaSign::Rotation(a_Block)  + 8) % 16);
			case BlockType::BirchSign:   return BirchSign::BirchSign(    (BirchSign::Rotation(a_Block)   + 8) % 16);
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign((CrimsonSign::Rotation(a_Block) + 8) % 16);
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign((DarkOakSign::Rotation(a_Block) + 8) % 16);
			case BlockType::JungleSign:  return JungleSign::JungleSign(   (JungleSign::Rotation(a_Block) + 8) % 16);
			case BlockType::OakSign:     return OakSign::OakSign(         (OakSign::Rotation(a_Block)    + 8) % 16);
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(   (SpruceSign::Rotation(a_Block) + 8) % 16);
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(   (WarpedSign::Rotation(a_Block) + 8) % 16);
			default: return a_Block;
		}
	}




	// TODO: Sanity Check
	virtual BlockState MirrorYZ(BlockState a_Block) const override
	{
		// Mirrors signs over the YZ plane (East-West Mirroring)
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(  (AcaciaSign::Rotation(a_Block)  + 8) % 16);
			case BlockType::BirchSign:   return BirchSign::BirchSign(    (BirchSign::Rotation(a_Block)   + 8) % 16);
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign((CrimsonSign::Rotation(a_Block) + 8) % 16);
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign((DarkOakSign::Rotation(a_Block) + 8) % 16);
			case BlockType::JungleSign:  return JungleSign::JungleSign(   (JungleSign::Rotation(a_Block) + 8) % 16);
			case BlockType::OakSign:     return OakSign::OakSign(         (OakSign::Rotation(a_Block)    + 8) % 16);
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(   (SpruceSign::Rotation(a_Block) + 8) % 16);
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(   (WarpedSign::Rotation(a_Block) + 8) % 16);
			default: return a_Block;
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 13;
	}
} ;




