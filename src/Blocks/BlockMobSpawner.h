
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





	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
		const cEntity * a_Digger
	) const override
	{
		if (a_Digger == nullptr)
		{
			return;
		}
		if (!a_Digger->IsPlayer())
		{
			return;
		}

		const auto Player = static_cast<const cPlayer *>(a_Digger);
		auto Handler = Player->GetEquippedItem().GetHandler();
		if (!Player->IsGameModeSurvival() || !Handler->CanHarvestBlock(E_BLOCK_MOB_SPAWNER))
		{
			return;
		}

		auto & Random = GetRandomProvider();
		int Reward = 15 + Random.RandInt(14) + Random.RandInt(14);
		a_WorldInterface.SpawnSplitExperienceOrbs(Vector3d(0.5, 0.5, 0.5) + a_BlockPos, Reward);
	}
} ;
