
// BlockInfested.h

#include "../Entities/Player.h"

/* This Block Handler describes the blocks spawning silver fishes. Mojang calls them monster egg */

class cBlockInfestedHandler final:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static void SpawnSilverfish(cWorldInterface & a_WorldInterface, Vector3i a_BlockPos)
	{
		auto Pos = Vector3f(a_BlockPos.x - 0.5f, a_BlockPos.y - 0.5f, a_BlockPos.z - 0.5f);
		// TODO: only display animation if the difficulty allows mob spawns - Add when difficulty is implemented
		// Spawn Silverfish
		a_WorldInterface.SpawnMob(Pos.x, Pos.y, Pos.z, mtSilverfish, false);
		// Play particle
		a_WorldInterface.GetBroadcastManager().BroadcastParticleEffect("explode", Pos, Vector3f(), 0.1f, 50);
	}

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		switch (a_BlockMeta)
		{
			case E_META_SILVERFISH_EGG_STONE:
			{
				if (ToolHasSilkTouch(a_Tool))
				{
					return cItem(E_BLOCK_STONE);
				}
				else
				{
					return cItem(E_BLOCK_COBBLESTONE);
				}
			}
			case E_META_SILVERFISH_EGG_COBBLESTONE:          return cItem(E_BLOCK_COBBLESTONE);
			case E_META_SILVERFISH_EGG_STONE_BRICK:          return cItem(E_BLOCK_STONE_BRICKS);
			case E_META_SILVERFISH_EGG_MOSSY_STONE_BRICK:    return cItem(E_BLOCK_STONE_BRICKS, 1, E_META_STONE_BRICK_MOSSY);
			case E_META_SILVERFISH_EGG_CRACKED_STONE_BRICK:  return cItem(E_BLOCK_STONE_BRICKS, 1, E_META_STONE_BRICK_CRACKED);
			case E_META_SILVERFISH_EGG_CHISELED_STONE_BRICK: return cItem(E_BLOCK_STONE_BRICKS, 1, E_META_STONE_BRICK_ORNAMENT);
		}
		return {};
	}





	virtual void OnBroken(
			cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
			Vector3i a_BlockPos,
			BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
			const cEntity * a_Digger
	) const override
	{
		if (a_Digger != nullptr)
		{
			if (a_Digger->IsPlayer())
			{
				const auto Player = static_cast<const cPlayer *>(a_Digger);
				if (Player->IsGameModeCreative())
				{
					return;
				}
			}
			if (a_Digger->IsMob())
			{
				return;
			}
		}
		SpawnSilverfish(a_WorldInterface, a_BlockPos);
	}


	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		return 11;
	}
} ;

