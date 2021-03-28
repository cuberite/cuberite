
// BlockInfested.h

// Declares the cBlockInfestedHandler class representing the handler for Silverfish blocks (Mojang calls them Monster Eggs)

#include "../Entities/Player.h"





class cBlockInfestedHandler final:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	static void SpawnSilverfish(cWorldInterface & a_WorldInterface, Vector3i a_BlockPos)
	{
		// TODO: only display animation if the difficulty allows mob spawns - Add when difficulty is implemented

		const auto Position = Vector3f(a_BlockPos.x + 0.5f, a_BlockPos.y, a_BlockPos.z + 0.5f);
		a_WorldInterface.SpawnMob(Position.x, Position.y, Position.z, mtSilverfish, false);
		a_WorldInterface.GetBroadcastManager().BroadcastParticleEffect("explode", Position, Vector3f(), 0.1f, 50);
	}


	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		switch (a_BlockMeta)
		{
			case E_META_SILVERFISH_EGG_STONE:
			{
				if (ToolHasSilkTouch(a_Tool))
				{
					return { E_BLOCK_STONE };
				}
				else
				{
					return { E_BLOCK_COBBLESTONE };
				}
			}
			case E_META_SILVERFISH_EGG_COBBLESTONE:          return { E_BLOCK_COBBLESTONE };
			case E_META_SILVERFISH_EGG_STONE_BRICK:          return { E_BLOCK_STONE_BRICKS };
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
