
#pragma once

#include "Blocks/BlockHandler.h"
#include "Blocks/Mixins.h"
#include "Entities/Player.h"
#include "UI/AnvilWindow.h"





class cBlockAnvilHandler final :
	public cYawRotator<cBlockHandler, 0x03, 0x03, 0x00, 0x01, 0x02>
{
	using Super = cYawRotator<cBlockHandler, 0x03, 0x03, 0x00, 0x01, 0x02>;

public:

	using Super::Super;

	static constexpr eBlockFace GetFacing(BlockState a_Block)
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

	/** Rotates a given block counter-clockwise. */
	BlockState RotateCCW(BlockState a_Block) const
	{
		auto Facing = RotateBlockFaceCCW(GetFacing(a_Block));
		switch (a_Block.Type())
		{
			case BlockType::Anvil:        return Block::Anvil::Anvil(Facing);
			case BlockType::ChippedAnvil: return Block::ChippedAnvil::ChippedAnvil(Facing);
			case BlockType::DamagedAnvil: return Block::DamagedAnvil::DamagedAnvil(Facing);
			default: return a_Block;
		}
	}

	/** Rotates a given block clockwise. */
	BlockState RotateCW(BlockState a_Block) const
	{
		auto Facing = RotateBlockFaceCW(GetFacing(a_Block));
		switch (a_Block.Type())
		{
			case BlockType::Anvil:        return Block::Anvil::Anvil(Facing);
			case BlockType::ChippedAnvil: return Block::ChippedAnvil::ChippedAnvil(Facing);
			case BlockType::DamagedAnvil: return Block::DamagedAnvil::DamagedAnvil(Facing);
			default: return a_Block;
		}
	}

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		return cItem(m_BlockType, 1, a_BlockMeta >> 2);
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





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 6;
	}
} ;




