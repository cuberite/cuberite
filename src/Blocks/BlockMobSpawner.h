
#pragma once

#include "BlockHandler.h"
#include "../Items/ItemHandler.h"





class cBlockMobSpawnerHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		return a_ChunkInterface.UseBlockEntity(&a_Player, a_BlockPos.x, a_BlockPos.y, a_BlockPos.z);
	}





	virtual bool IsUseable() const override
	{
		return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// No pickups
		return {};
	}





	virtual void OnPlayerBrokeBlock(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta
	) const override
	{
		auto handler = a_Player.GetEquippedItem().GetHandler();
		if (!a_Player.IsGameModeSurvival() || !handler->CanHarvestBlock(E_BLOCK_MOB_SPAWNER))
		{
			return;
		}

		auto & random = GetRandomProvider();
		int reward = 15 + random.RandInt(14) + random.RandInt(14);
		a_WorldInterface.SpawnSplitExperienceOrbs(Vector3d(0.5, 0.5, 0.5) + a_BlockPos, reward);
	}
} ;
