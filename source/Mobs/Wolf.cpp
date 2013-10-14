
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wolf.h"
#include "../World.h"
#include "../Entities/Player.h"





cWolf::cWolf(void) :
	super("Wolf", 95, "mob.wolf.hurt", "mob.wolf.death", 0.6, 0.8),
	m_bIsAngry(false),
	m_bIsTame(false),
	m_bIsSitting(false),
	m_bIsBegging(false)
{
}





void cWolf::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);
	if (!m_bIsTame)
	{
		m_bIsAngry = true;
	}
	m_World->BroadcastEntityMetadata(*this); // Broadcast health and possibly angry face
}





void cWolf::OnRightClicked(cPlayer & a_Player)
{
	if ((!m_bIsTame) && (!m_bIsAngry))
	{
		if (a_Player.GetEquippedItem().m_ItemType == E_ITEM_BONE)
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
			}

			if (m_World->GetTickRandomNumber(10) == 5)
			{
				SetMaxHealth(20);
				m_bIsTame = true;
				m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_WOLF_TAMED);
			}
			else
			{
				m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_WOLF_TAMING);
			}
		}
	}
	else if (m_bIsTame)
	{
		if (m_bIsSitting)
		{
			m_bIsSitting = false;
		}
		else
		{
			m_bIsSitting = true;
		}
	}
	
	m_World->BroadcastEntityMetadata(*this);
}




