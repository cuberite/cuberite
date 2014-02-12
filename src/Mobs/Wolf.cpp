
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wolf.h"
#include "../World.h"
#include "../Entities/Player.h"





cWolf::cWolf(void) :
	super("Wolf", mtWolf, "mob.wolf.hurt", "mob.wolf.death", 0.6, 0.8),
	m_IsSitting(false),
	m_IsTame(false),
	m_IsBegging(false),
	m_IsAngry(false),
	m_OwnerName(""),
	m_CollarColor(14)
{
}





void cWolf::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);
	if (!m_IsTame)
	{
		m_IsAngry = true;
	}
	m_World->BroadcastEntityMetadata(*this); // Broadcast health and possibly angry face
}




void cWolf::Attack(float a_Dt)
{
	UNUSED(a_Dt);

	if ((m_Target != NULL) && (m_Target->IsPlayer()))
	{
		if (((cPlayer *)m_Target)->GetName() != m_OwnerName)
		{
			super::Attack(a_Dt);
		}
	}
}





void cWolf::OnRightClicked(cPlayer & a_Player)
{
	if (!IsTame() && !IsAngry())
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
				SetOwner(a_Player.GetName());
				m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_WOLF_TAMED);
				m_World->BroadcastParticleEffect("heart", (float) GetPosX(), (float) GetPosY(), (float) GetPosZ(), 0, 0, 0, 0, 5);
			}
			else
			{
				m_World->BroadcastEntityStatus(*this, ENTITY_STATUS_WOLF_TAMING);
				m_World->BroadcastParticleEffect("smoke", (float) GetPosX(), (float) GetPosY(), (float) GetPosZ(), 0, 0, 0, 0, 5);
			}
		}
	}
	else if (IsTame())
	{
		if (a_Player.GetName() == m_OwnerName) // Is the player the owner of the dog?
		{
			if (a_Player.GetEquippedItem().m_ItemType == E_ITEM_DYE)
			{
				SetCollarColor(15 - a_Player.GetEquippedItem().m_ItemDamage);
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.GetInventory().RemoveOneEquippedItem();
				}
			} 
			else if (IsSitting()) 
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
	}
	else
	{
		super::Tick(a_Dt, a_Chunk);
	}
	
	// The wolf is sitting so don't move him at all.
	if (IsSitting())
	{
		m_bMovingToDestination = false;
	}

	cPlayer * a_Closest_Player = m_World->FindClosestPlayer(GetPosition(), (float)m_SightDistance);
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
				// Don't move to the player if the wolf is sitting.
				if (IsSitting())
				{
					m_bMovingToDestination = false;
				}
				else
				{
					m_bMovingToDestination = true;
				}
				Vector3d PlayerPos = a_Closest_Player->GetPosition();
				PlayerPos.y++;
				m_FinalDestination = PlayerPos;
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
		TickFollowPlayer();
	}
}





void cWolf::TickFollowPlayer()
{
	class cCallback :
		public cPlayerListCallback
	{
		virtual bool Item(cPlayer * a_Player) override
		{
			OwnerPos = a_Player->GetPosition();
			return false;
		}
	public:
		Vector3d OwnerPos;
	} Callback;
	if (m_World->DoWithPlayer(m_OwnerName, Callback))
	{
		// The player is present in the world, follow him:
		double Distance = (Callback.OwnerPos - GetPosition()).Length();
		if (Distance > 30)
		{
			if (!IsSitting())
			{
				Callback.OwnerPos.y = FindFirstNonAirBlockPosition(Callback.OwnerPos.x, Callback.OwnerPos.z);
				TeleportToCoords(Callback.OwnerPos.x, Callback.OwnerPos.y, Callback.OwnerPos.z);
			}
		}
		else
		{
			m_FinalDestination = Callback.OwnerPos;
			if (IsSitting())
			{
				m_bMovingToDestination = false;
			}
			else
			{
				m_bMovingToDestination = true;
			}
		}
	}
}




