
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
	cItemSpawnEggHandler(int a_ItemID) :
		cItemHandler(a_ItemID)
	{

	}


	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, cItem * a_Item, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		if (a_Dir < 0)
		{
			return false;
		}
		
		AddDirection(a_X, a_Y, a_Z, a_Dir);

		if (a_Dir == BLOCK_FACE_BOTTOM)
		{
			a_Y--;
		}

		cMonster * Monster = NULL;

		Monster = new cZombie();

		Monster->Initialize(a_World);
		Monster->TeleportTo(a_X + 0.5, a_Y, a_Z + 0.5);
		a_World->BroadcastSpawn(*Monster);

		a_Player->UseEquippedItem();
		return true;
	}
} ;




