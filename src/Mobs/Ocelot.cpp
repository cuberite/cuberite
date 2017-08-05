
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Ocelot.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Items/ItemHandler.h"
#include "Broadcaster.h"





cOcelot::cOcelot(void) :
	super("Ocelot", mtOcelot, "entity.cat.hurt", "entity.cat.death", 0.6, 0.8),
	m_IsSitting(false),
	m_IsTame(false),
	m_IsBegging(false),
	m_CatType(ctWildOcelot),
	m_OwnerName("")
{
}





void cOcelot::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (!IsTame() && !IsBaby())
	{
		if (m_CheckPlayerTickCount == 23)
		{
			cPlayer * a_Closest_Player = m_World->FindClosestPlayer(GetPosition(), 10, true);
			if (a_Closest_Player != nullptr)
			{
				cItems Items;
				GetBreedingItems(Items);
				if (Items.ContainsType(a_Closest_Player->GetEquippedItem().m_ItemType))
				{
					if (!IsBegging())
					{
						SetIsBegging(true);
						m_World->BroadcastEntityMetadata(*this);
					}

					MoveToPosition(a_Closest_Player->GetPosition());
				}
				else
				{
					if (IsBegging())
					{
						SetIsBegging(false);
						m_World->BroadcastEntityMetadata(*this);
					}
				}
			}

			m_CheckPlayerTickCount = 0;
		}
		else
		{
			m_CheckPlayerTickCount++;
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

	m_World->BroadcastEntityMetadata(*this);
}





void cOcelot::TickFollowPlayer()
{
	class cCallback :
		public cPlayerListCallback
	{
		virtual bool Item(cPlayer * a_Player) override
		{
			OwnerPos = a_Player->GetPosition();
			OwnerFlying = a_Player->IsFlying();
			return true;
		}
	public:
		Vector3d OwnerPos;
		bool OwnerFlying;
	} Callback;

	if (m_World->DoWithPlayerByUUID(m_OwnerUUID, Callback))
	{
		// The player is present in the world, follow him:
		double Distance = (Callback.OwnerPos - GetPosition()).Length();
		if (Distance > 12)
		{
			if (!Callback.OwnerFlying)
			{
				Callback.OwnerPos.y = FindFirstNonAirBlockPosition(Callback.OwnerPos.x, Callback.OwnerPos.z);
				TeleportToCoords(Callback.OwnerPos.x, Callback.OwnerPos.y, Callback.OwnerPos.z);
			}
		}
		if (Distance < 2)
		{
			StopMovingToPosition();
		}
		else
		{
			if (!Callback.OwnerFlying)
			{
				MoveToPosition(Callback.OwnerPos);
			}
		}
	}
}





void cOcelot::OnRightClicked(cPlayer & a_Player)
{
	if (!IsTame())
	{
		if (
			IsBegging() &&
			((a_Player.GetPosition() - GetPosition()).Length() <= 3)
		)
		{
			cItems Items;
			GetBreedingItems(Items);
			if (Items.ContainsType(a_Player.GetEquippedItem().m_ItemType))
			{
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.GetInventory().RemoveOneEquippedItem();
				}

				auto & Random = GetRandomProvider();

				if (Random.RandBool(1.0 / 3.0))
				{
					// Taming succeeded
					SetIsBegging(false);

					SetMaxHealth(20);
					SetIsTame(true);
					SetOwner(a_Player.GetName(), a_Player.GetUUID());
					SetCatType(static_cast<eCatType>(Random.RandInt<int>(1, 3)));
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
		else
		{
			super::OnRightClicked(a_Player);
		}
	}
	else if (a_Player.GetUUID() == m_OwnerUUID)
	{
		super::OnRightClicked(a_Player);
		SetIsSitting(!IsSitting());
	}
	m_World->BroadcastEntityMetadata(*this);
}





void cOcelot::SpawnOn(cClientHandle & a_ClientHandle)
{
	super::SpawnOn(a_ClientHandle);
	if (!IsBaby() && GetRandomProvider().RandBool(1.0 / 7.0))
	{
		m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), m_MobType, true);
		m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), m_MobType, true);
	}
}




