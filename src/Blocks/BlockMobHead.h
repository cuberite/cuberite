
#pragma once

#include "BlockEntity.h"





class cBlockMobHeadHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	static inline bool IsBlockMobHead(BlockState a_Block)
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
			case BlockType::PiglinHead:
			case BlockType::PiglinWallHead:
				return true;
			default: return false;
		}
	}

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;




