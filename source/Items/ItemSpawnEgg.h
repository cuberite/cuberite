
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

		Monster = new cZombie();

		Monster->Initialize(a_World);
		Monster->TeleportTo(a_BlockX + 0.5, a_BlockY, a_BlockZ + 0.5);
		a_World->BroadcastSpawn(*Monster);

		a_Player->UseEquippedItem();
		return true;
	}
} ;




