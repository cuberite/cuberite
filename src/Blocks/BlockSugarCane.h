
#pragma once

#include "BlockPlant.h"





class cBlockSugarCaneHandler final :
	public cBlockPlant<false>
{
	using Super = cBlockPlant<false>;

public:

	using Super::Super;

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		const auto BelowPos = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(BelowPos))
		{
			return false;
		}

		switch (a_Chunk.GetBlock(BelowPos).Type())
		{
			case BlockType::Dirt:
			case BlockType::GrassBlock:
			case BlockType::Farmland:
			case BlockType::Sand:
			{
				static const Vector3i Coords[] =
				{
					{-1, 0,  0},
					{ 1, 0,  0},
					{ 0, 0, -1},
					{ 0, 0,  1},
				} ;
				for (size_t i = 0; i < ARRAYCOUNT(Coords); i++)
				{
					BlockState Block;
					if (!a_Chunk.UnboundedRelGetBlock(a_Position + Coords[i], Block))
					{
						// Too close to the edge, cannot simulate
						return true;
					}
					if ((Block.Type() == BlockType::Water) || (Block.Type() == BlockType::FrostedIce))
					{
						return true;
					}
				}  // for i - Coords[]
				// Not directly neighboring a water block
				return false;
			}
			case BlockType::SugarCane:
			{
				return true;
			}
			default: return false;
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, char a_NumStages = 1) const override
	{
		// Check the total height of the sugarcane blocks here:
		auto top = a_RelPos.addedY(1);
		while (
			cChunkDef::IsValidHeight(top) &&
			(a_Chunk.GetBlock(top).Type() == BlockType::SugarCane)
		)
		{
			++top.y;
		}
		auto bottom = a_RelPos.addedY(-1);
		while (
			cChunkDef::IsValidHeight(bottom) &&
			(a_Chunk.GetBlock(bottom).Type() == BlockType::SugarCane)
		)
		{
			--bottom.y;
		}
		const auto NumStages = static_cast<unsigned char>(std::clamp<char>(a_NumStages, 0, std::numeric_limits<char>::max()));

		// Grow by at most a_NumStages, but no more than max height:
		auto toGrow = std::min<unsigned char>(NumStages, static_cast<unsigned char>(a_Chunk.GetWorld()->GetMaxSugarcaneHeight() + 1 - (top.y - bottom.y)));
		Vector3i TopYPos(a_RelPos.x, top.y, a_RelPos.z);
		for (int i = 0; i < toGrow; i++)
		{
			if (cBlockAirHandler::IsBlockAir(a_Chunk.GetBlock(TopYPos.addedY(i))))
			{
				a_Chunk.SetBlock(TopYPos.addedY(i), Block::SugarCane::SugarCane());
			}
			else
			{
				return i;
			}
		}  // for i
		return toGrow;
	}

	virtual PlantAction CanGrow(cChunk & a_Chunk, Vector3i a_RelPos) const override
	{
		// Only allow growing if there's an air block above:
		if (((a_RelPos.y + 1) < cChunkDef::Height) && (cBlockAirHandler::IsBlockAir(a_Chunk.GetBlock(a_RelPos.addedY(1)))))
		{
			return Super::CanGrow(a_Chunk, a_RelPos);
		}
		return paStay;
	}
} ;




