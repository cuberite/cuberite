
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"





class cBlockSpongeHandler : public cBlockHandler
{
public:
	cBlockSpongeHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		if (a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) == E_META_SPONGE_NORMAL)
		{
			AbsorbWater(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);
		}
	}


	virtual void OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, const sSetBlock & a_BlockChange) override
	{
		if (a_BlockChange.m_BlockMeta == E_META_SPONGE_NORMAL)
		{
			int BlockX = a_BlockChange.m_ChunkX * cChunkDef::Width + a_BlockChange.m_RelX;
			int BlockZ = a_BlockChange.m_ChunkZ * cChunkDef::Width + a_BlockChange.m_RelZ;

			AbsorbWater(a_ChunkInterface, BlockX, a_BlockChange.m_RelY, BlockZ);
		}
	}


	void AbsorbWater(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		std::list<Vector3i> Blocks;
		std::list<Vector3i> WaterBlocks;
		Blocks.push_back(Vector3i(a_BlockX, a_BlockY, a_BlockZ));

		int CurrentRadius = 0;
		while (!Blocks.empty() && (CurrentRadius < 6) && (WaterBlocks.size() <= 64))
		{
			Vector3i BlockLoc = Blocks.front();
			Blocks.pop_front();

			bool ChangedBlock = false;
			for (size_t i = 0; i <= 5; ++i)
			{
				eBlockFace Face = static_cast<eBlockFace>(i);

				int BlockX = BlockLoc.x, BlockY = BlockLoc.y, BlockZ = BlockLoc.z;
				AddFaceDirection(BlockX, BlockY, BlockZ, Face);

				if (IsBlockWater(a_ChunkInterface.GetBlock(BlockX, BlockY, BlockZ)))
				{
					Blocks.push_back(Vector3i(BlockX, BlockY, BlockZ));
					WaterBlocks.push_back(Vector3i(BlockX, BlockY, BlockZ));
					a_ChunkInterface.FastSetBlock(BlockX, BlockY, BlockZ, E_BLOCK_AIR, 0);

					ChangedBlock = true;
				}
			}

			if (ChangedBlock)
			{
				CurrentRadius++;
			}
		}

		if (WaterBlocks.size() > 0)
		{
			// Change the sponge to a wet sponge.
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, E_META_SPONGE_WET);

			// Remove the water
			for (auto Block : WaterBlocks)
			{
				a_ChunkInterface.SetBlock(Block.x, Block.y, Block.z, E_BLOCK_AIR, 0);
			}

			// ChunkInterface doesn't have a BroadcastSoundParticleEffect() method.
			// a_World->BroadcastSoundParticleEffect(2001, (int) a_BlockX * 8.f), (int) (a_BlockY * 8.f), (int) (a_BlockZ * 8.f), E_BLOCK_STATIONARY_WATER);
		}
	}

} ;




