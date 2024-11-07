#pragma once

#include "BlockPlant.h"
#include "../BlockInfo.h"





class cBlockCactusHandler final :
	public cClearMetaOnDrop<cBlockPlant<false>>
{
	using Super = cClearMetaOnDrop<cBlockPlant<false>>;

public:

	using Super::Super;

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		const auto SurfacePosition = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(SurfacePosition))
		{
			return false;
		}
		BLOCKTYPE Surface = a_Chunk.GetBlock(SurfacePosition);
		if ((Surface != E_BLOCK_SAND) && (Surface != E_BLOCK_CACTUS))
		{
			// Cactus can only be placed on sand and itself
			return false;
		}

		// Check surroundings. Cacti may ONLY be surrounded by non-solid blocks
		static const Vector3i Coords[] =
		{
			{-1, 0,  0},
			{ 1, 0,  0},
			{ 0, 0, -1},
			{ 0, 0,  1},
		};
		for (size_t i = 0; i < ARRAYCOUNT(Coords); i++)
		{
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			if (
				a_Chunk.UnboundedRelGetBlock(a_Position + Coords[i], BlockType, BlockMeta) &&
				(
					cBlockInfo::IsSolid(BlockType) ||
					(BlockType == E_BLOCK_LAVA) ||
					(BlockType == E_BLOCK_STATIONARY_LAVA)
				)
			)
			{
				return false;
			}
		}  // for i - Coords[]

		return true;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}




	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) const override
	{
		// Check the total height of the cacti blocks here:
		auto Top = a_RelPos.addedY(1);
		while (
			cChunkDef::IsValidHeight(Top) &&
			(a_Chunk.GetBlock(Top) == E_BLOCK_CACTUS)
		)
		{
			Top.y++;
		}
		auto Bottom = a_RelPos.addedY(-1);
		while (
			cChunkDef::IsValidHeight(Bottom) &&
			(a_Chunk.GetBlock(Bottom) == E_BLOCK_CACTUS)
		)
		{
			--Bottom.y;
		}

		// Refuse if already too high:
		auto numToGrow = std::min(a_NumStages, a_Chunk.GetWorld()->GetMaxCactusHeight() + 1 - (Top.y - Bottom.y));
		if (numToGrow <= 0)
		{
			return 0;
		}

		BLOCKTYPE blockType;
		for (int i = 0; i < numToGrow; ++i)
		{
			auto NewTop = Top.addedY(i);
			if (!a_Chunk.UnboundedRelGetBlockType(NewTop, blockType) || (blockType != E_BLOCK_AIR))
			{
				// Cannot grow there
				return i;
			}

			a_Chunk.UnboundedRelFastSetBlock(NewTop, E_BLOCK_CACTUS, 0);

			// Check surroundings. Cacti may ONLY be surrounded by non-solid blocks; if they aren't, drop as pickup and bail out the growing
			static const Vector3i neighborOffsets[] =
			{
				{-1, 0,  0},
				{ 1, 0,  0},
				{ 0, 0, -1},
				{ 0, 0,  1},
			} ;
			for (const auto & ofs: neighborOffsets)
			{
				if (
					a_Chunk.UnboundedRelGetBlockType(NewTop + ofs, blockType) &&
					(
						cBlockInfo::IsSolid(blockType) ||
						(blockType == E_BLOCK_LAVA) ||
						(blockType == E_BLOCK_STATIONARY_LAVA)
					)
				)
				{
					// Remove the cactus
					auto absPos = a_Chunk.RelativeToAbsolute(NewTop);
					a_Chunk.GetWorld()->DropBlockAsPickups(absPos);
					return i + 1;
				}
			}  // for neighbor
		}  // for i - numToGrow
		return numToGrow;
	}

	virtual PlantAction CanGrow(cChunk & a_Chunk, Vector3i a_RelPos) const override
	{
		// Only allow growing if there's an air block above:
		const auto RelPosAbove = a_RelPos.addedY(1);
		if (cChunkDef::IsValidHeight(RelPosAbove) && (a_Chunk.GetBlock(RelPosAbove) == E_BLOCK_AIR))
		{
			return Super::CanGrow(a_Chunk, a_RelPos);
		}
		return paStay;
	}
} ;




