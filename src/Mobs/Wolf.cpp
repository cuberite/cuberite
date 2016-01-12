
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wolf.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Items/ItemHandler.h"
#include "Broadcaster.h"





cWolf::cWolf(void) :
	super("Wolf", mtWolf, "mob.wolf.hurt", "mob.wolf.death", 0.6, 0.8),
	m_IsSitting(false),
	m_IsTame(false),
	m_IsBegging(false),
	m_IsAngry(false),
	m_OwnerName(""),
	m_CollarColor(E_META_DYE_ORANGE)
{
	m_RelativeWalkSpeed = 2;
}





bool cWolf::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if (!m_IsTame)
	{
		m_IsAngry = true;
	}
	m_World->BroadcastEntityMetadata(*this);  // Broadcast health and possibly angry face
	return true;
}




bool cWolf::Attack(std::chrono::milliseconds a_Dt)
{
	UNUSED(a_Dt);

	if ((m_Target != nullptr) && (m_Target->IsPlayer()))
	{
		if (static_cast<cPlayer *>(m_Target)->GetName() != m_OwnerName)
		{
			return super::Attack(a_Dt);
		}
		else
		{
			m_Target = nullptr;
		}
	}
	else
	{
		return super::Attack(a_Dt);
	}

	return false;
}





void cWolf::NearbyPlayerIsFighting(cPlayer * a_Player, cEntity * a_Opponent)
{
	if ((m_Target == nullptr) && (a_Player->GetName() == m_OwnerName) && !IsSitting())
	{
		m_Target = a_Opponent;
	}

}





void cWolf::OnRightClicked(cPlayer & a_Player)
{
	if (!IsTame() && !IsAngry())
	{
		// If the player is holding a bone, try to tame the wolf:
		if (a_Player.GetEquippedItem().m_ItemType == E_ITEM_BONE)
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
			}

			if (m_World->GetTickRandomNumber(7) == 0)
			{
				// Taming succeeded
				SetMaxHealth(20);
				SetIsTame(true);
				SetOwner(a_Player.GetName(), a_Player.GetUUID());
				m_World->BroadcastEntityStatus(*this, esWolfTamed);
				m_World->GetBroadcaster().BroadcastParticleEffect("heart", static_cast<Vector3f>(GetPosition()), Vector3f{}, 0, 5);
			}
			else
			{
				// Taming failed
				m_World->BroadcastEntityStatus(*this, esWolfTaming);
				m_World->GetBroadcaster().BroadcastParticleEffect("smoke", static_cast<Vector3f>(GetPosition()), Vector3f{}, 0, 5);
			}
		}
	}
	else if (IsTame())
	{
		// Feed the wolf, restoring its health, or dye its collar:
		switch (a_Player.GetEquippedItem().m_ItemType)
		{
			case E_ITEM_RAW_BEEF:
			case E_ITEM_STEAK:
			case E_ITEM_RAW_PORKCHOP:
			case E_ITEM_COOKED_PORKCHOP:
			case E_ITEM_RAW_CHICKEN:
			case E_ITEM_COOKED_CHICKEN:
			case E_ITEM_ROTTEN_FLESH:
			{
				if (m_Health < m_MaxHealth)
				{
					Heal(ItemHandler(a_Player.GetEquippedItem().m_ItemType)->GetFoodInfo().FoodLevel);
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.GetInventory().RemoveOneEquippedItem();
					}
				}
				break;
			}
			case E_ITEM_DYE:
			{
				if (a_Player.GetName() == m_OwnerName)  // Is the player the owner of the dog?
				{
					SetCollarColor(a_Player.GetEquippedItem().m_ItemDamage);
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.GetInventory().RemoveOneEquippedItem();
					}
				}
				break;
			}
			default:
			{
				if (a_Player.GetName() == m_OwnerName)  // Is the player the owner of the dog?
				{
					SetIsSitting(!IsSitting());
				}
			}
		}
	}

	m_World->BroadcastEntityMetadata(*this);
}





void cWolf::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (!IsAngry())
	{
		cMonster::Tick(a_Dt, a_Chunk);
	}
	else
	{
		super::Tick(a_Dt, a_Chunk);
	}

	if (m_Target == nullptr)
	{
		cPlayer * a_Closest_Player = m_World->FindClosestPlayer(GetPosition(), static_cast<float>(m_SightDistance));
		if (a_Closest_Player != nullptr)
		{
			switch (a_Closest_Player->GetEquippedItem().m_ItemType)
			{
				case E_ITEM_BONE:
				case E_ITEM_RAW_BEEF:
				case E_ITEM_STEAK:
				case E_ITEM_RAW_CHICKEN:
				case E_ITEM_COOKED_CHICKEN:
				case E_ITEM_ROTTEN_FLESH:
				case E_ITEM_RAW_PORKCHOP:
				case E_ITEM_COOKED_PORKCHOP:
				{
					if (!IsBegging())
					{
						SetIsBegging(true);
						m_World->BroadcastEntityMetadata(*this);
					}

					m_FinalDestination = a_Closest_Player->GetPosition();  // So that we will look at a player holding food

					// Don't move to the player if the wolf is sitting.
					if (!IsSitting())
					{
						MoveToPosition(a_Closest_Player->GetPosition());
					}

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
	}
	else
	{
		if (IsSitting())
		{
			m_Target = nullptr;
		}
		else
		{
			MoveToPosition(m_Target->GetPosition());
			if (TargetIsInRange())
			{
				Attack(a_Dt);
			}
		}
	}

	if (IsTame() && !IsSitting())
	{
		TickFollowPlayer();
	}
	else if (IsSitting())
	{
		StopMovingToPosition();
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
		if (Distance > 20)
		{
			Callback.OwnerPos.y = FindFirstNonAirBlockPosition(Callback.OwnerPos.x, Callback.OwnerPos.z);
			TeleportToCoords(Callback.OwnerPos.x, Callback.OwnerPos.y, Callback.OwnerPos.z);
			m_Target = nullptr;
		}
		if (Distance < 2)
		{
			if (m_Target == nullptr)
			{
				StopMovingToPosition();
			}
		}
		else
		{
			if (m_Target == nullptr)
			{
				MoveToPosition(Callback.OwnerPos);
			}
		}
	}
}



void cWolf::InStateIdle(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (!IsTame())
	{
		cMonster::InStateIdle(a_Dt, a_Chunk);
	}
}


