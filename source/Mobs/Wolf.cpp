
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wolf.h"
#include "../World.h"
#include "../Entities/Player.h"





cWolf::cWolf(void) :
	super("Wolf", mtWolf, "mob.wolf.hurt", "mob.wolf.death", 0.6, 0.8),
	m_bIsAngry(false),
	m_bIsTame(false),
	m_bIsSitting(false),
	m_bIsBegging(false),
	m_bOwner(NULL)
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
	if ((!IsTame()) && (!IsAngry()))
	{
		if (a_Player.GetEquippedItem().m_ItemType == E_ITEM_BONE)
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
			}

			if (m_World->GetTickRandomNumber(7) == 0)
			{
				SetMaxHealth(20);
				SetIsTame(true);
				SetOwner(&a_Player);
				m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_WOLF_TAMED);
			}
			else
			{
				m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_WOLF_TAMING);
			}
		}
	}
	else if (IsTame())
	{
		if (m_bOwner != NULL && a_Player.GetUniqueID() == m_bOwner->GetUniqueID()) // Is the player the owner of the dog?
		{
			if (IsSitting())
			{
				SetIsSitting(false);
			}
			else
			{
				SetIsSitting(true);
			}
		}
	}
	
	m_World->BroadcastEntityMetadata(*this);
}





void cWolf::Tick(float a_Dt, cChunk & a_Chunk)
{
	if (!IsAngry())
	{
		cMonster::Tick(a_Dt, a_Chunk);
	} else {
		super::Tick(a_Dt, a_Chunk);
	}

	if (IsSitting())
	{
		m_bMovingToDestination = false;
	}

	cPlayer * a_Closest_Player = FindClosestPlayer();
	if (a_Closest_Player != NULL)
	{
		switch (a_Closest_Player->GetEquippedItem().m_ItemType)
		{
			case E_ITEM_BONE:
			case E_ITEM_RAW_BEEF:
			case E_ITEM_STEAK:
			case E_ITEM_RAW_CHICKEN:
			case E_ITEM_COOKED_CHICKEN:
			case E_ITEM_ROTTEN_FLESH:
			{
				if (!IsBegging())
				{
					SetIsBegging(true);
					m_World->BroadcastEntityMetadata(*this);
				}
				Vector3f a_NewDestination = a_Closest_Player->GetPosition();
				a_NewDestination.y = a_NewDestination.y + 1; // Look at the head of the player, not his feet.
				m_Destination = Vector3f(a_NewDestination);
				m_bMovingToDestination = false;
				break;
			}
			default:
			{
				if (IsBegging())
				{
					SetIsBegging(false);
					m_World->BroadcastEntityMetadata(*this);
				}
			}
		}
	}

	if (IsTame())
	{
		if (m_bOwner != NULL)
		{
			Vector3f OwnerCoords = m_bOwner->GetPosition();
			double Distance = (OwnerCoords - GetPosition()).Length();
			if (Distance < 3)
			{
				m_bMovingToDestination = false;
			} else if((Distance > 30) && (!IsSitting())) {
				TeleportToEntity(*m_bOwner);
			} else {
				m_Destination = OwnerCoords;
			}
		}
	}
	
}