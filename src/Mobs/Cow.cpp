
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cow.h"
#include "../Entities/Player.h"







cCow::cCow(void) :
	super("Cow", mtCow, "mob.cow.hurt", "mob.cow.hurt", 0.9, 1.3)
{
}





void cCow::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_LEATHER);
	AddRandomDropItem(a_Drops, 1, 3, IsOnFire() ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
}





void cCow::OnRightClicked(cPlayer & a_Player)
{
	if ((a_Player.GetEquippedItem().m_ItemType == E_ITEM_BUCKET))
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
			a_Player.GetInventory().AddItem(E_ITEM_MILK);
		}
	}
}

void cCow::Tick(float a_Dt, cChunk & a_Chunk)
{	
	super::Tick(a_Dt, a_Chunk);
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
