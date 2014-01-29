
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Mooshroom.h"
#include "../Entities/Player.h"





// TODO: Milk Cow





cMooshroom::cMooshroom(void) :
	super("Mooshroom", mtMooshroom, "mob.cow.hurt", "mob.cow.hurt", 0.9, 1.3)
{
}





void cMooshroom::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_LEATHER);
	AddRandomDropItem(a_Drops, 1, 3, IsOnFire() ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
}


void cMooshroom::Tick(float a_Dt, cChunk & a_Chunk)
{
	cPlayer * a_Closest_Player = m_World->FindClosestPlayer(GetPosition(), (float)m_SightDistance);
	if (a_Closest_Player != NULL)
	{
		if (a_Closest_Player->GetEquippedItem().m_ItemType == E_ITEM_WHEAT)
		{
			if (!IsBegging())
			{
				m_IsBegging = true;
				m_World->BroadcastEntityMetadata(*this);
			}
			Vector3d PlayerPos = a_Closest_Player->GetPosition();
			PlayerPos.y++;
			m_FinalDestination = PlayerPos;
		}
		else
		{
			if (IsBegging())
			{
				m_IsBegging = false;
				m_World->BroadcastEntityMetadata(*this);
			}
		}
	}
}


