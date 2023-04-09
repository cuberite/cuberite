
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

	static constexpr unsigned char ROTATION_MAX = 16;
	static constexpr unsigned char ROTATION_STEP = 4;
	static constexpr unsigned char ROTATION_MIRROR = 8;

	static constexpr bool IsBlockSignPost(BlockState a_Block)
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

	static constexpr unsigned char GetRotation(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return Block::AcaciaSign::Rotation(a_Block);
			case BlockType::BirchSign:   return Block::BirchSign::Rotation(a_Block);
			case BlockType::CrimsonSign: return Block::CrimsonSign::Rotation(a_Block);
			case BlockType::DarkOakSign: return Block::DarkOakSign::Rotation(a_Block);
			case BlockType::JungleSign:  return Block::JungleSign::Rotation(a_Block);
			case BlockType::OakSign:     return Block::OakSign::Rotation(a_Block);
			case BlockType::SpruceSign:  return Block::SpruceSign::Rotation(a_Block);
			case BlockType::WarpedSign:  return Block::WarpedSign::Rotation(a_Block);
			default: return 0;
		}
	}

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		return cItem(E_ITEM_SIGN, 1, 0);
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		if (a_Position.y <= 0)
		{
			return false;
		}

		BLOCKTYPE Type = a_Chunk.GetBlock(a_Position.addedY(-1));
		return (Type == E_BLOCK_SIGN_POST) || (Type == E_BLOCK_WALLSIGN) || cBlockInfo::IsSolid(Type);
	}





	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) const override
	{
		return (a_Meta + 4) & 0x0f;
	}




	// Todo: Override this, once the BlockState system is implemented
	BlockState RotateCW(BlockState a_Block) const
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
			case BlockType::BirchSign:   return BirchSign::BirchSign(     static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign( static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign( static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
			case BlockType::JungleSign:  return JungleSign::JungleSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
			case BlockType::OakSign:     return OakSign::OakSign(         static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_STEP, ROTATION_MAX)));
			default: return a_Block;
		}
	}





	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) const override
	{
		return (a_Meta + 12) & 0x0f;
	}




	// Todo: Override this, once the BlockState system is implemented
	BlockState RotateCCW(BlockState a_Block) const
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
			case BlockType::BirchSign:   return BirchSign::BirchSign(     static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign( static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign( static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
			case BlockType::JungleSign:  return JungleSign::JungleSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
			case BlockType::OakSign:     return OakSign::OakSign(         static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) - ROTATION_STEP, ROTATION_MAX)));
			default: return a_Block;
		}
	}





	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) const override
	{
		// Mirrors signs over the XY plane (North-South Mirroring)

		// There are 16 meta values which correspond to different directions.
		// These values are equated to angles on a circle; 0x08 = 180 degrees.
		return (a_Meta < 0x08) ? (0x08 - a_Meta) : (0x18 - a_Meta);
	}




	// Todo: Override this, once the BlockState system is implemented
	BlockState MirrorXY(BlockState a_Block) const
	{
		// Mirrors signs over the XY plane (North-South Mirroring)
		using namespace Block;

		auto Rotation = GetRotation(a_Block);
		// If the banner is aligned with the X axis, it can't be mirrored:
		if ((Rotation == 0) || (Rotation == 8))
		{
			return a_Block;
		}

		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::BirchSign:   return BirchSign::BirchSign(     static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign( static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign( static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::JungleSign:  return JungleSign::JungleSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::OakSign:     return OakSign::OakSign(         static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			default: return a_Block;
		}
	}





	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) const override
	{
		// Mirrors signs over the YZ plane (East-West Mirroring)

		// There are 16 meta values which correspond to different directions.
		// These values are equated to angles on a circle; 0x10 = 360 degrees.
		return 0x0f - a_Meta;
	}





	// Todo: Override this, once the BlockState system is implemented
	BlockState MirrorYZ(BlockState a_Block) const
	{
		// Mirrors signs over the XY plane (North-South Mirroring)
		using namespace Block;

		auto Rotation = GetRotation(a_Block);
		// If the banner is aligned with the Z axis, it can't be mirrored:
		if ((Rotation == 4) || (Rotation == 12))
		{
			return a_Block;
		}

		switch (a_Block.Type())
		{
			case BlockType::AcaciaSign:  return AcaciaSign::AcaciaSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::BirchSign:   return BirchSign::BirchSign(     static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::CrimsonSign: return CrimsonSign::CrimsonSign( static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::DarkOakSign: return DarkOakSign::DarkOakSign( static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::JungleSign:  return JungleSign::JungleSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::OakSign:     return OakSign::OakSign(         static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::SpruceSign:  return SpruceSign::SpruceSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			case BlockType::WarpedSign:  return WarpedSign::WarpedSign(   static_cast<unsigned char>(std::remainder(GetRotation(a_Block) + ROTATION_MIRROR, ROTATION_MAX)));
			default: return a_Block;
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 13;
	}
} ;




