
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemSpawnEggHandler : public cItemHandler
{
public:
	cItemSpawnEggHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		if (a_BlockFace < 0)
		{
			return false;
		}

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);

		if (a_BlockFace == BLOCK_FACE_YM)
		{
			a_BlockY--;
		}

		eMonsterType MonsterType = ItemDamageToMonsterType(a_Item.m_ItemDamage);
		if (
			(MonsterType != mtInvalidType) &&  // Valid monster type
			(a_World->SpawnMob(a_BlockX + 0.5, a_BlockY, a_BlockZ + 0.5, MonsterType, false) != cEntity::INVALID_ID))  // Spawning succeeded
		{
			if (!a_Player->IsGameModeCreative())
			{
				// The mob was spawned, "use" the item:
				a_Player->GetInventory().RemoveOneEquippedItem();
			}
			return true;
		}

		return false;
	}


	/** Converts the Spawn egg item damage to the monster type to spawn.
	Returns mtInvalidType for invalid damage values. */
	static eMonsterType ItemDamageToMonsterType(short a_ItemDamage)
	{
		switch (a_ItemDamage)
		{
			case E_META_SPAWN_EGG_BAT:           return mtBat;
			case E_META_SPAWN_EGG_BLAZE:         return mtBlaze;
			case E_META_SPAWN_EGG_CAVE_SPIDER:   return mtCaveSpider;
			case E_META_SPAWN_EGG_CHICKEN:       return mtChicken;
			case E_META_SPAWN_EGG_COW:           return mtCow;
			case E_META_SPAWN_EGG_CREEPER:       return mtCreeper;
			case E_META_SPAWN_EGG_ENDERMAN:      return mtEnderman;
			case E_META_SPAWN_EGG_GHAST:         return mtGhast;
			case E_META_SPAWN_EGG_GUARDIAN:      return mtGuardian;
			case E_META_SPAWN_EGG_HORSE:         return mtHorse;
			case E_META_SPAWN_EGG_MAGMA_CUBE:    return mtMagmaCube;
			case E_META_SPAWN_EGG_MOOSHROOM:     return mtMooshroom;
			case E_META_SPAWN_EGG_OCELOT:        return mtOcelot;
			case E_META_SPAWN_EGG_PIG:           return mtPig;
			case E_META_SPAWN_EGG_RABBIT:        return mtRabbit;
			case E_META_SPAWN_EGG_SHEEP:         return mtSheep;
			case E_META_SPAWN_EGG_SILVERFISH:    return mtSilverfish;
			case E_META_SPAWN_EGG_SKELETON:      return mtSkeleton;
			case E_META_SPAWN_EGG_SLIME:         return mtSlime;
			case E_META_SPAWN_EGG_SPIDER:        return mtSpider;
			case E_META_SPAWN_EGG_SQUID:         return mtSquid;
			case E_META_SPAWN_EGG_VILLAGER:      return mtVillager;
			case E_META_SPAWN_EGG_WITCH:         return mtWitch;
			case E_META_SPAWN_EGG_WOLF:          return mtWolf;
			case E_META_SPAWN_EGG_ZOMBIE:        return mtZombie;
			case E_META_SPAWN_EGG_ZOMBIE_PIGMAN: return mtZombiePigman;
		}
		return mtInvalidType;
	}
} ;




