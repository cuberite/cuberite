
#pragma once

#include "BlockHandler.h"
#include "BlockLeaves.h"
#include "../FastRandom.h"
#include "Mixins/DirtLikeUnderneath.h"





class cBlockSaplingHandler final :
	public cDirtLikeUnderneath<cBlockHandler>
{
	using Super = cDirtLikeUnderneath<cBlockHandler>;

public:

	using Super::Super;

	static inline bool IsBlockSapling(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSapling:
			case BlockType::BirchSapling:
			case BlockType::JungleSapling:
			case BlockType::DarkOakSapling:
			case BlockType::OakSapling:
			case BlockType::SpruceSapling:
			case BlockType::CherrySapling:
			case BlockType::PaleOakSapling:
				return true;
			default: return false;
		}
	}

	static inline unsigned char GetSaplingStage(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSapling:  return AcaciaSapling::Stage(a_Block);
			case BlockType::BirchSapling:   return BirchSapling::Stage(a_Block);
			case BlockType::JungleSapling:  return JungleSapling::Stage(a_Block);
			case BlockType::DarkOakSapling: return DarkOakSapling::Stage(a_Block);
			case BlockType::OakSapling:     return OakSapling::Stage(a_Block);
			case BlockType::SpruceSapling:  return SpruceSapling::Stage(a_Block);
			case BlockType::CherrySapling:  return CherrySapling::Stage(a_Block);
			case BlockType::PaleOakSapling:  return PaleOakSapling::Stage(a_Block);
			default: return 0;
		}
	}

	static inline BlockState SetSaplingState(BlockState a_Block, unsigned char a_State)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::AcaciaSapling:  return AcaciaSapling::AcaciaSapling(a_State);
			case BlockType::BirchSapling:   return BirchSapling::BirchSapling(a_State);
			case BlockType::JungleSapling:  return JungleSapling::JungleSapling(a_State);
			case BlockType::DarkOakSapling: return DarkOakSapling::DarkOakSapling(a_State);
			case BlockType::OakSapling:     return OakSapling::OakSapling(a_State);
			case BlockType::SpruceSapling:  return SpruceSapling::SpruceSapling(a_State);
			case BlockType::CherrySapling:  return CherrySapling::CherrySapling(a_State);
			case BlockType::PaleOakSapling:  return PaleOakSapling::PaleOakSapling(a_State);
			default: return a_Block;
		}
	}

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const BlockState a_Self) const override
	{
		return cChunkDef::IsValidHeight(a_Position) && IsBlockMaterialDirt(a_Self);
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		auto GrowthStage = GetSaplingStage(a_Chunk.GetBlock(a_RelPos));
		auto Light = std::max(a_Chunk.GetBlockLight(a_RelPos), a_Chunk.GetTimeAlteredLight(a_Chunk.GetSkyLight(a_RelPos)));

		// Only grow if we have the right amount of light
		if (Light > 8)
		{
			auto & Random = GetRandomProvider();
			// Only grow if we are in the right growth stage and have the right amount of space around them.
			if ((GrowthStage > 0) && Random.RandBool(0.45) && CanGrowAt(a_Chunk, a_RelPos.x, a_RelPos.y, a_RelPos.z, a_Chunk.GetBlock(a_RelPos)))
			{
				auto WorldPos = a_Chunk.RelativeToAbsolute(a_RelPos);
				a_Chunk.GetWorld()->GrowTree(WorldPos);
			}
			// Only move to the next growth stage if we haven't gone there yet
			else if ((GrowthStage == 0) && Random.RandBool(0.45))
			{
				a_Chunk.FastSetBlock(a_RelPos, SetSaplingState(a_Chunk.GetBlock(a_RelPos), 1));
			}
		}
	}





	static bool CanGrowAt(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ, BlockState a_Block)
	{
		int CheckHeight = 0;
		bool LargeTree = false;

		// Get the height to check against
		switch (a_Block.Type())
		{
			// Acacias don't need horizontal clearance
			case BlockType::AcaciaSapling:
			{
				if (!IsLargeTree(a_Chunk, a_RelX, a_RelY, a_RelZ, a_Block))
				{
					return false;
				}
				CheckHeight = 7;
				LargeTree = true;
				break;
			}
			case BlockType::BirchSapling:
			{
				CheckHeight = 6;
				break;
			}
			// Dark Oaks don't need horizontal clearance
			case BlockType::DarkOakSapling:
			{
				if (!IsLargeTree(a_Chunk, a_RelX, a_RelY, a_RelZ, a_Block))
				{
					return false;
				}
				CheckHeight = 7;
				LargeTree = true;
				break;
			}
			case BlockType::JungleSapling:
			{
				CheckHeight = 7;
				if (IsLargeTree(a_Chunk, a_RelX, a_RelY, a_RelZ, a_Block))
				{
					CheckHeight = 13;
					LargeTree = true;
				}
				break;
			}
			case BlockType::OakSapling:
			{
				CheckHeight = 5;
				break;
			}
			case BlockType::SpruceSapling:
			{
				CheckHeight = 7;
				if (IsLargeTree(a_Chunk, a_RelX, a_RelY, a_RelZ, a_Block))
				{
					CheckHeight = 16;
					LargeTree = true;
				}
				break;
			}
			default: return false;
			// TODO: implement Azalea, pale oak, cherry, and mangrove saplings and bamboo
		}
		// We should always get a valid CheckHeight
		ASSERT(CheckHeight != 0);

		// Don't grow a tree if we don't have enough space left above it in the chunk
		if ((a_RelY + CheckHeight) > cChunkDef::Height)
		{
			return false;
		}
		bool CanGrow = true;

		// Validate the neighbor blocks. They cannot be solid.
		BlockState BlockToCheck = Block::Air::Air();
		a_Chunk.UnboundedRelGetBlock(a_RelX - 1, a_RelY, a_RelZ, BlockToCheck);
		CanGrow = CanGrow && cBlockInfo::IsTransparent(BlockToCheck);

		a_Chunk.UnboundedRelGetBlock(a_RelX + 1, a_RelY, a_RelZ, BlockToCheck);
		CanGrow = CanGrow && cBlockInfo::IsTransparent(BlockToCheck);

		a_Chunk.UnboundedRelGetBlock(a_RelX, a_RelY, a_RelZ - 1, BlockToCheck);
		CanGrow = CanGrow && cBlockInfo::IsTransparent(BlockToCheck);

		a_Chunk.UnboundedRelGetBlock(a_RelX, a_RelY, a_RelZ + 1, BlockToCheck);
		CanGrow = CanGrow && cBlockInfo::IsTransparent(BlockToCheck);

		while (CheckHeight && CanGrow)
		{
			BlockToCheck = a_Chunk.GetBlock(a_RelX, a_RelY + CheckHeight, a_RelZ);
			CanGrow = CanGrow && (cBlockAirHandler::IsBlockAir(BlockToCheck) || cBlockLeavesHandler::IsBlockLeaves(BlockToCheck));

			// We have to BlockToCheck above the neighboring saplings as well
			if (LargeTree)
			{
				a_Chunk.UnboundedRelGetBlock(a_RelX + 1, a_RelY + CheckHeight, a_RelZ, BlockToCheck);
				CanGrow = CanGrow && (cBlockAirHandler::IsBlockAir(BlockToCheck) || cBlockLeavesHandler::IsBlockLeaves(BlockToCheck));

				a_Chunk.UnboundedRelGetBlock(a_RelX, a_RelY + CheckHeight, a_RelZ + 1, BlockToCheck);
				CanGrow = CanGrow && (cBlockAirHandler::IsBlockAir(BlockToCheck) || cBlockLeavesHandler::IsBlockLeaves(BlockToCheck));

				a_Chunk.UnboundedRelGetBlock(a_RelX + 1, a_RelY + CheckHeight, a_RelZ + 1, BlockToCheck);
				CanGrow = CanGrow && (cBlockAirHandler::IsBlockAir(BlockToCheck) || cBlockLeavesHandler::IsBlockLeaves(BlockToCheck));
			}

			--CheckHeight;
		}

		return CanGrow;
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, char a_NumStages = 1) const override
	{
		auto Self = a_Chunk.GetBlock(a_RelPos);
		auto OldState = GetSaplingStage(Self);
		int res = 0;

		// Try to increase the sapling's growState:
		if (OldState < 1)
		{
			++OldState;
			a_Chunk.FastSetBlock(a_RelPos, SetSaplingState(Self, OldState));
			if (a_NumStages == 1)
			{
				// Only asked to grow one stage, which we did. Bail out.
				return 1;
			}
			res = 1;
		}

		// The sapling is grown, now it becomes a tree:
		a_Chunk.GetWorld()->GrowTreeFromSapling(a_Chunk.RelativeToAbsolute(a_RelPos));
		return res + 1;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}

	static bool IsLargeTree(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ, BlockState a_Block)
	{
		BlockState Self;
		bool LargeTree = true;
		a_Chunk.UnboundedRelGetBlock(a_RelX + 1, a_RelY, a_RelZ, Self);
		LargeTree = LargeTree && (Self.Type() == a_Block.Type());

		a_Chunk.UnboundedRelGetBlock(a_RelX + 1, a_RelY, a_RelZ + 1, Self);
		LargeTree = LargeTree && (Self.Type() == a_Block.Type());

		a_Chunk.UnboundedRelGetBlock(a_RelX, a_RelY, a_RelZ + 1, Self);
		LargeTree = LargeTree && (Self.Type() == a_Block.Type());

		return LargeTree;
	}
} ;




