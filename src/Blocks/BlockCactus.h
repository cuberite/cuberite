#pragma once

#include "BlockPlant.h"
#include "../BlockInfo.h"





class cBlockCactusHandler final :
	public cBlockPlant<false>
{
	using Super = cBlockPlant<false>;

public:

	using Super::Super;

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		const auto SurfacePosition = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(SurfacePosition))
		{
			return false;
		}
		auto Surface = a_Chunk.GetBlock(SurfacePosition);
		if ((Surface.Type() != BlockType::Sand) && (Surface.Type() != BlockType::Cactus))
		{
			// Cactus can only be placed on sand and itself
			return false;
		}

		// Check surroundings. Cacti may ONLY be surrounded by non-solid blocks
		static const std::array<Vector3i, 4> Coords =
		{
			Vector3i(-1, 0,  0),
			Vector3i( 1, 0,  0),
			Vector3i( 0, 0, -1),
			Vector3i( 0, 0,  1)
		};
		for (const auto & Offset : Coords)
		{
			BlockState BlockToCheck;
			if (
				a_Chunk.UnboundedRelGetBlock(a_Position + Offset, BlockToCheck) &&
				(
					cBlockInfo::IsSolid(BlockToCheck) ||
					(BlockToCheck.Type() == BlockType::Lava)
				)
			)
			{
				return false;
			}
		}

		return true;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}




	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, char a_NumStages = 1) const override
	{
		// Check the total height of the cacti blocks here:
		auto Top = a_RelPos.y + 1;
		while (
			(Top < cChunkDef::Height) &&
			(a_Chunk.GetBlock({a_RelPos.x, Top, a_RelPos.z}).Type() == BlockType::Cactus)
		)
		{
			++Top;
		}
		int Bottom = a_RelPos.y - 1;
		while (
			(Bottom > 0) &&
			(a_Chunk.GetBlock({a_RelPos.x, Bottom, a_RelPos.z}).Type() == BlockType::Cactus)
		)
		{
			--Bottom;
		}

		// Refuse if already too high:
		auto NumToGrow = std::min<char>(a_NumStages, static_cast<char>(a_Chunk.GetWorld()->GetMaxCactusHeight() + 1 - (Top - Bottom)));
		if (NumToGrow <= 0)
		{
			return 0;
		}

		BlockState BlockToReplace;
		for (int i = 0; i < NumToGrow; ++i)
		{
			Vector3i NewPos(a_RelPos.x, Top + i, a_RelPos.z);
			if (!a_Chunk.UnboundedRelGetBlock(NewPos, BlockToReplace) || (BlockToReplace.Type() != BlockType::Air))
			{
				// Cannot grow there
				return i;
			}

			a_Chunk.UnboundedRelFastSetBlock(NewPos, Block::Cactus::Cactus());

			// Check surroundings. Cacti may ONLY be surrounded by non-solid blocks; if they aren't, drop as pickup and bail out the growing
			static constexpr std::array<Vector3i, 4> NeighborOffsets =
			{
				Vector3i(-1, 0,  0),
				Vector3i( 1, 0,  0),
				Vector3i( 0, 0, -1),
				Vector3i( 0, 0,  1),
			} ;
			for (const auto & Offset: NeighborOffsets)
			{
				BlockState BlockToCheck;
				if (
					a_Chunk.UnboundedRelGetBlock(NewPos + Offset, BlockToCheck) &&
					(
						cBlockInfo::IsSolid(BlockToCheck) ||
						(BlockToCheck.Type() == BlockType::Lava)
					)
				)
				{
					// Remove the cactus
					auto AbsPos = a_Chunk.RelativeToAbsolute(NewPos);
					a_Chunk.GetWorld()->DropBlockAsPickups(AbsPos);
					return i + 1;
				}
			}  // for neighbor
		}  // for i - NumToGrow
		return NumToGrow;
	}

	virtual PlantAction CanGrow(cChunk & a_Chunk, Vector3i a_RelPos) const override
	{
		// Only allow growing if there's an air block above:
		const auto RelPosAbove = a_RelPos.addedY(1);
		if (cChunkDef::IsValidHeight(RelPosAbove) && IsBlockAir(a_Chunk.GetBlock(RelPosAbove)))
		{
			return Super::CanGrow(a_Chunk, a_RelPos);
		}
		return paStay;
	}
} ;




