
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
				return true;
			default: return false;
		}
	}

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
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




