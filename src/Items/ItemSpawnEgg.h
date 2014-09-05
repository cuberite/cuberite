
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


	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
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

		cMonster::eType MonsterType = ItemDamageToMonsterType(a_Item.m_ItemDamage);
		if (
			(MonsterType != cMonster::mtInvalidType) &&  // Valid monster type
			(a_World->SpawnMob(a_BlockX + 0.5, a_BlockY, a_BlockZ + 0.5, MonsterType) >= 0))  // Spawning succeeded
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
	static cMonster::eType ItemDamageToMonsterType(short a_ItemDamage)
	{
		switch (a_ItemDamage)
		{
			case E_META_SPAWN_EGG_BAT:           return cMonster::mtBat;
			case E_META_SPAWN_EGG_BLAZE:         return cMonster::mtBlaze;
			case E_META_SPAWN_EGG_CAVE_SPIDER:   return cMonster::mtCaveSpider;
			case E_META_SPAWN_EGG_CHICKEN:       return cMonster::mtChicken;
			case E_META_SPAWN_EGG_COW:           return cMonster::mtCow;
			case E_META_SPAWN_EGG_CREEPER:       return cMonster::mtCreeper;
			case E_META_SPAWN_EGG_ENDERMAN:      return cMonster::mtEnderman;
			case E_META_SPAWN_EGG_GHAST:         return cMonster::mtGhast;
			case E_META_SPAWN_EGG_HORSE:         return cMonster::mtHorse;
			case E_META_SPAWN_EGG_MAGMA_CUBE:    return cMonster::mtMagmaCube;
			case E_META_SPAWN_EGG_MOOSHROOM:     return cMonster::mtMooshroom;
			case E_META_SPAWN_EGG_OCELOT:        return cMonster::mtOcelot;
			case E_META_SPAWN_EGG_PIG:           return cMonster::mtPig;
			case E_META_SPAWN_EGG_SHEEP:         return cMonster::mtSheep;
			case E_META_SPAWN_EGG_SILVERFISH:    return cMonster::mtSilverfish;
			case E_META_SPAWN_EGG_SKELETON:      return cMonster::mtSkeleton;
			case E_META_SPAWN_EGG_SLIME:         return cMonster::mtSlime;
			case E_META_SPAWN_EGG_SPIDER:        return cMonster::mtSpider;
			case E_META_SPAWN_EGG_SQUID:         return cMonster::mtSquid;
			case E_META_SPAWN_EGG_VILLAGER:      return cMonster::mtVillager;
			case E_META_SPAWN_EGG_WITCH:         return cMonster::mtWitch;
			case E_META_SPAWN_EGG_WOLF:          return cMonster::mtWolf;
			case E_META_SPAWN_EGG_ZOMBIE:        return cMonster::mtZombie;
			case E_META_SPAWN_EGG_ZOMBIE_PIGMAN: return cMonster::mtZombiePigman;
		}
		return cMonster::mtInvalidType;
	}
} ;




