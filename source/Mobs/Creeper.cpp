
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Creeper.h"
#include "Skeleton.h"
#include "../World.h"
#include <cstdlib>



cCreeper::cCreeper(void) :
	super("Creeper", 50, "mob.creeper.say", "mob.creeper.say", 0.6, 1.8)
{
}





void cCreeper::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	//If a_killer isn't Skeleton - killerSkeleton == NULL
	cSkeleton *killerSkeleton = dynamic_cast<cSkeleton *>(a_Killer);
	if (killerSkeleton != NULL)
	{
		int randomItem = rand() % 12 + E_ITEM_FIRST_DISC;
		AddRandomDropItem(a_Drops, 1, 1, randomItem);
	}
	else
	{
		AddRandomDropItem(a_Drops, 0, 2, E_ITEM_GUNPOWDER);
	}
}

void cCreeper::Attack(float a_Dt)
{
	
	Vector3f Pos = Vector3f( GetPosition() );
	Vector3f Their = Vector3f( m_Target->GetPosition() );
	if ((Their - Pos).Length() <= 3)
	{
		cMonster::Attack(a_Dt);
		GetWorld()->DoExplosiontAt(4.0, GetPosX() + 0.49, GetPosY() + 0.49, GetPosZ() + 0.49, false, esCreeper, this);
		Destroy(true);
	} else
	{
		MoveToPosition(Their + Vector3f(0, 0.65f, 0));
	}
	
}



