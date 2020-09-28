
// BlockInfested.h

/* This Block Handler describes the blocks spawning silver fishes. Mojang calls them monster egg */

class cBlockInfestedHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		switch (a_BlockMeta)
		{
			case E_META_SILVERFISH_EGG_STONE:
			{
				if (a_Tool->m_Enchantments.GetLevel(cEnchantments::enchSilkTouch))
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





	virtual void OnPlayerBrokeBlock(
			cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
			cPlayer & a_Player,
			Vector3i a_BlockPos,
			BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta
	) const override
	{
		SpawnSilverfish(a_ChunkInterface, a_WorldInterface, a_BlockPos);
	}


	ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		return 11;
	}




	void SpawnSilverfish(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, Vector3i a_BlockPos) const
	{
		// TODO: only display animation if the difficulty allows mob spawns
		// Spawn Silverfish
		a_WorldInterface.SpawnMob(a_BlockPos.x - 0.5f , a_BlockPos.y - 0.5f, a_BlockPos.z - 0.5f, mtSilverfish, false);
		// Play particle
		a_WorldInterface.GetBroadcastManager().BroadcastParticleEffect("explode", a_BlockPos, {0.5f, 0.5f, 0.5f}, 0.1, 50);
	}
} ;

