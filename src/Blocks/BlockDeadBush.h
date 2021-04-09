
#pragma once

#include "BlockHandler.h"





class cBlockDeadBushHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, BlockState a_Block) const override
	{
		return true;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}

		auto BelowBlock = a_Chunk.GetBlock(a_RelPos.addedY(-1));
		switch (BelowBlock.Type())
		{
			case BlockType::Clay:
			case BlockType::Terracotta:
			case BlockType::BlackTerracotta:
			case BlockType::BlueTerracotta:
			case BlockType::BrownTerracotta:
			case BlockType::CyanTerracotta:
			case BlockType::GrayTerracotta:
			case BlockType::GreenTerracotta:
			case BlockType::LightBlueTerracotta:
			case BlockType::LightGrayTerracotta:
			case BlockType::LimeTerracotta:
			case BlockType::MagentaTerracotta:
			case BlockType::OrangeTerracotta:
			case BlockType::PinkTerracotta:
			case BlockType::PurpleTerracotta:
			case BlockType::RedTerracotta:
			case BlockType::WhiteTerracotta:
			case BlockType::YellowTerracotta:
			case BlockType::Sand:
			{
				return true;
			}
			default: return false;
		}
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// If cutting down with shears, drop self:
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == E_ITEM_SHEARS))
		{
			return cItem(Item::DeadBush);
		}

		// Drop 0-3 sticks:
		auto chance = GetRandomProvider().RandInt<char>(3);
		if (chance > 0)
		{
			return cItem(Item::Stick, chance, 0);
		}
		return {};
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;
