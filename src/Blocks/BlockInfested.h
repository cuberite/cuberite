
// BlockInfested.h

// Declares the cBlockInfestedHandler class representing the handler for Silverfish blocks (Mojang calls them Monster Eggs)

#include "../Entities/Player.h"





class cBlockInfestedHandler final:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockInfested(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::InfestedChiseledStoneBricks:
			case BlockType::InfestedCobblestone:
			case BlockType::InfestedCrackedStoneBricks:
			case BlockType::InfestedMossyStoneBricks:
			case BlockType::InfestedStone:
				return true;
			default: return false;
		}
	}

private:

	static void SpawnSilverfish(cWorldInterface & a_WorldInterface, Vector3i a_BlockPos)
	{
		// TODO: only display animation if the difficulty allows mob spawns - Add when difficulty is implemented

		const auto Position = Vector3f(a_BlockPos.x + 0.5f, static_cast<float>(a_BlockPos.y), a_BlockPos.z + 0.5f);
		a_WorldInterface.SpawnMob(Position.x, Position.y, Position.z, mtSilverfish, false);
		a_WorldInterface.GetBroadcastManager().BroadcastParticleEffect("explode", Position, Vector3f(), 0.1f, 50);
	}


	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		switch (a_Block.Type())
		{
			case BlockType::InfestedStone:
			{
				if (ToolHasSilkTouch(a_Tool))
				{
					return { Item::Stone };
				}
				else
				{
					return { Item::Cobblestone };
				}
			}
			case BlockType::InfestedCobblestone:          return { Item::Cobblestone };
			case BlockType::InfestedStoneBricks:          return { Item::StoneBricks };
			case BlockType::InfestedMossyStoneBricks:    return { Item::MossyStoneBricks };
			case BlockType::InfestedCrackedStoneBricks:  return { Item::CrackedStoneBricks };
			case BlockType::InfestedChiseledStoneBricks: return { Item::ChiseledStoneBricks };
			default: return {};
		}
	}


	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BlockState a_Block,
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
			if (a_Digger->IsMob() || a_Digger->IsTNT())
			{
				return;
			}
		}
		SpawnSilverfish(a_WorldInterface, a_BlockPos);
	}


	virtual ColourID GetMapBaseColourID() const override
	{
		return 11;
	}
} ;
