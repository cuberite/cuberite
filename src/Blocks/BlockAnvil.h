
#pragma once

#include "BlockHandler.h"
#include "Mixins/Mixins.h"
#include "../Entities/Player.h"
#include "../UI/AnvilWindow.h"





class cBlockAnvilHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline eBlockFace GetFacing(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Anvil:        return Block::Anvil::Facing(a_Block);
			case BlockType::ChippedAnvil: return Block::ChippedAnvil::Facing(a_Block);
			case BlockType::DamagedAnvil: return Block::DamagedAnvil::Facing(a_Block);
			default: return BLOCK_FACE_NONE;
		}
	}

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(Item::Anvil, 1, 0);
	}





	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		cWindow * Window = new cAnvilWindow(a_BlockPos);
		a_Player.OpenWindow(*Window);
		return true;
	}





	virtual bool IsUseable() const override
	{
		return true;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 6;
	}
} ;




