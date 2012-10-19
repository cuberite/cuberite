
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"

// Mobs:
#include "../Mobs/Chicken.h"
#include "../Mobs/Spider.h"
#include "../Mobs/Cow.h"
#include "../Mobs/Squid.h"
#include "../Mobs/Wolf.h"
#include "../Mobs/Slime.h"
#include "../Mobs/Skeleton.h"
#include "../Mobs/Silverfish.h"
#include "../Mobs/Pig.h"
#include "../Mobs/Sheep.h"
#include "../Mobs/Zombie.h"
#include "../Mobs/Enderman.h"
#include "../Mobs/Creeper.h"
#include "../Mobs/Cavespider.h"
#include "../Mobs/Ghast.h"
#include "../Mobs/Zombiepigman.h"
#include "../Mobs/Villager.h"
#include "../Mobs/Ocelot.h"
#include "../Mobs/Mooshroom.h"
#include "../Mobs/Magmacube.h"
#include "../Mobs/Blaze.h"





class cItemSpawnEggHandler : public cItemHandler
{
public:
	cItemSpawnEggHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}


	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, cItem * a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		if (a_Dir < 0)
		{
			return false;
		}
		
		AddDirection(a_BlockX, a_BlockY, a_BlockZ, a_Dir);

		if (a_Dir == BLOCK_FACE_BOTTOM)
		{
			a_BlockY--;
		}

		cMonster * Monster = NULL;

		switch (a_Item->m_ItemDamage)
		{
			case E_META_SPAWN_EGG_BLAZE:
			{
				Monster = new cBlaze();
				break;
			}
			case E_META_SPAWN_EGG_CAVE_SPIDER:
			{
				Monster = new cCavespider();
				break;
			}
			case E_META_SPAWN_EGG_CHICKEN:
			{
				Monster = new cChicken();
				break;
			}
			case E_META_SPAWN_EGG_COW:
			{
				Monster = new cCow();
				break;
			}
			case E_META_SPAWN_EGG_CREEPER:
			{
				Monster = new cCreeper();
				break;
			}
			case E_META_SPAWN_EGG_ENDERMAN:
			{
				Monster = new cEnderman();
				break;
			}
			case E_META_SPAWN_EGG_GHAST:
			{
				Monster = new cGhast();
				break;
			}
			case E_META_SPAWN_EGG_MAGMA_CUBE:
			{
				Monster = new cMagmacube();
				break;
			}
			case E_META_SPAWN_EGG_MOOSHROOM:
			{
				Monster = new cMooshroom();
				break;
			}
			case E_META_SPAWN_EGG_OCELOT:
			{
				Monster = new cOcelot();
				break;
			}
			case E_META_SPAWN_EGG_PIG:
			{
				Monster = new cPig();
				break;
			}
			case E_META_SPAWN_EGG_SHEEP:
			{
				Monster = new cSheep();
				break;
			}
			case E_META_SPAWN_EGG_SILVERFISH:
			{
				Monster = new cSilverfish();
				break;
			}
			case E_META_SPAWN_EGG_SKELETON:
			{
				Monster = new cSkeleton();
				break;
			}
			case E_META_SPAWN_EGG_SLIME:
			{
				Monster = new cSlime();
				break;
			}
			case E_META_SPAWN_EGG_SPIDER:
			{
				Monster = new cSpider();
				break;
			}
			case E_META_SPAWN_EGG_SQUID:
			{
				Monster = new cSquid();
				break;
			}
			case E_META_SPAWN_EGG_VILLAGER:
			{
				Monster = new cVillager();
				break;
			}
			case E_META_SPAWN_EGG_WOLF:
			{
				Monster = new cWolf();
				break;
			}
			case E_META_SPAWN_EGG_ZOMBIE:
			{
				Monster = new cZombie();
				break;
			}
			case E_META_SPAWN_EGG_ZOMBIE_PIGMAN:
			{
				Monster = new cZombiepigman();
				break;
			}
			default:
			{
				return false;
			}
		}
		Monster->Initialize(a_World);
		Monster->TeleportTo(a_BlockX + 0.5, a_BlockY, a_BlockZ + 0.5);
		a_World->BroadcastSpawn(*Monster);

		a_Player->UseEquippedItem();
		return true;
	}
} ;




