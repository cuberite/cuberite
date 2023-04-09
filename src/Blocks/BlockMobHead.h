
#pragma once

#include "BlockEntity.h"





class cBlockMobHeadHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

	static constexpr bool IsBlockMobHead(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::CreeperHead:
			case BlockType::CreeperWallHead:
			case BlockType::DragonHead:
			case BlockType::DragonWallHead:
			case BlockType::PlayerHead:
			case BlockType::PlayerWallHead:
			case BlockType::SkeletonSkull:
			case BlockType::SkeletonWallSkull:
			case BlockType::WitherSkeletonSkull:
			case BlockType::WitherSkeletonWallSkull:
			case BlockType::ZombieHead:
			case BlockType::ZombieWallHead:
				return true;
			default: return false;
		}
	}

	static constexpr bool IsWallHead(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::CreeperWallHead:
			case BlockType::DragonWallHead:
			case BlockType::PlayerWallHead:
			case BlockType::SkeletonWallSkull:
			case BlockType::WitherSkeletonWallSkull:
			case BlockType::ZombieWallHead:
				return true;
			default: return false;
		}
	}

	/** Don't use this function for wall heads. */
	static constexpr unsigned char GetRotation(BlockState a_Block)
	{
		ASSERT(!IsWallHead(a_Block));
		switch (a_Block.Type())
		{
			case BlockType::CreeperHead: return Block::CreeperHead::Rotation(a_Block);
			case BlockType::DragonHead: return Block::DragonHead::Rotation(a_Block);
			case BlockType::PlayerHead: return Block::PlayerHead::Rotation(a_Block);
			case BlockType::SkeletonSkull: return Block::SkeletonSkull::Rotation(a_Block);
			case BlockType::WitherSkeletonSkull: return Block::WitherSkeletonSkull::Rotation(a_Block);
			case BlockType::ZombieHead: return Block::ZombieHead::Rotation(a_Block);
			default: return 0;
		}
	}

	/** Don't use this function for floor heads. */
	static constexpr eBlockFace GetFacing(BlockState a_Block)
	{
		ASSERT(IsWallHead(a_Block));
		switch (a_Block.Type())
		{
			case BlockType::CreeperWallHead: return Block::CreeperWallHead::Facing(a_Block);
			case BlockType::DragonWallHead: return Block::DragonWallHead::Facing(a_Block);
			case BlockType::PlayerWallHead: return Block::PlayerWallHead::Facing(a_Block);
			case BlockType::SkeletonWallSkull: return Block::SkeletonWallSkull::Facing(a_Block);
			case BlockType::WitherSkeletonWallSkull: return Block::WitherSkeletonWallSkull::Facing(a_Block);
			case BlockType::ZombieWallHead: return Block::ZombieWallHead::Facing(a_Block);
			default: return BLOCK_FACE_NONE;
		}
	}

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




