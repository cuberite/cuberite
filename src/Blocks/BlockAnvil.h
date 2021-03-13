
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"
#include "../Entities/Player.h"
#include "../UI/AnvilWindow.h"





class cBlockAnvilHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockAnvil(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Anvil:
			case BlockType::ChippedAnvil:
			case BlockType::DamagedAnvil:
			{
				return true;
			}
			default: return false;
		}
	}

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

	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override
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





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		if (!Super::GetPlacementBlockTypeMeta(a_ChunkInterface, a_Player, a_PlacedBlockPos, a_ClickedBlockFace, a_CursorPos, a_Block))
		{
			return false;
		}

		Block::Anvil::Anvil(RotationToBlockFace(a_Player.GetYaw()));
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




