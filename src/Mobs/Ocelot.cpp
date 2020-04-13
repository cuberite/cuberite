
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Ocelot.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Items/ItemHandler.h"
#include "../BoundingBox.h"





// TODO: Ocelots should have a chance of spawning with two kittens
/*
	if (!IsBaby() && GetRandomProvider().RandBool(1.0 / 7.0))
	{
		m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), m_MobType, true);
		m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), m_MobType, true);
	}
*/

cOcelot::cOcelot(void) :
	Super("Ocelot", mtOcelot, "entity.cat.hurt", "entity.cat.death", "entity.cat.ambient", 0.6, 0.8),
	m_IsSitting(false),
	m_IsTame(false),
	m_IsBegging(false),
	m_CatType(ctWildOcelot),
	m_OwnerName("")
{
}





void cOcelot::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (!IsTame() && !IsBaby())
	{
		if (m_CheckPlayerTickCount == 23)
		{
			m_World->DoWithNearestPlayer(GetPosition(), 10, [&](cPlayer & a_Player) -> bool
			{
				cItems Items;
				GetBreedingItems(Items);
				if (Items.ContainsType(a_Player.GetEquippedItem().m_ItemType))
				{
					if (!IsBegging())
					{
						SetIsBegging(true);
						m_World->BroadcastEntityMetadata(*this);
					}

					MoveToPosition(a_Player.GetPosition());
				}
				else
				{
					if (IsBegging())
					{
						SetIsBegging(false);
						m_World->BroadcastEntityMetadata(*this);
					}
				}

				return true;
			}, true);
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
	Vector3d OwnerPos;
	bool OwnerFlying = false;
	auto Callback = [&](cPlayer & a_Player)
	{
		OwnerPos = a_Player.GetPosition();
		OwnerFlying = a_Player.IsFlying();
		return true;
	};

	if (m_World->DoWithPlayerByUUID(m_OwnerUUID, Callback))
	{
		// The player is present in the world, follow him:
		double Distance = (OwnerPos - GetPosition()).Length();
		if (Distance > 12)
		{
			if (!OwnerFlying)
			{
				OwnerPos.y = FindFirstNonAirBlockPosition(OwnerPos.x, OwnerPos.z);
				TeleportToCoords(OwnerPos.x, OwnerPos.y, OwnerPos.z);
			}
		}
		if (Distance < 2)
		{
			StopMovingToPosition();
		}
		else
		{
			if (!OwnerFlying)
			{
				MoveToPosition(OwnerPos);
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
					m_World->BroadcastParticleEffect("heart", static_cast<Vector3f>(GetPosition()), Vector3f{}, 0, 5);
				}
				else
				{
					// Taming failed
					m_World->BroadcastEntityStatus(*this, esWolfTaming);
					m_World->BroadcastParticleEffect("smoke", static_cast<Vector3f>(GetPosition()), Vector3f{}, 0, 5);
				}
			}
		}
		else
		{
			Super::OnRightClicked(a_Player);
		}
	}
	else if (a_Player.GetUUID() == m_OwnerUUID)
	{
		Super::OnRightClicked(a_Player);
		SetIsSitting(!IsSitting());
	}
	m_World->BroadcastEntityMetadata(*this);
}





bool cOcelot::IsCatSittingOnBlock(cWorld * a_World, Vector3d a_BlockPosition)
{
	return a_World->ForEachEntityInBox(
		cBoundingBox(Vector3d(a_BlockPosition.x, a_BlockPosition.y + 1, a_BlockPosition.z), 1),
		[=](cEntity & a_Entity)
		{
			return (
				(a_Entity.GetEntityType() == cEntity::etMonster) &&
				(static_cast<cMonster &>(a_Entity).GetMobType() == eMonsterType::mtOcelot) &&
				(static_cast<cOcelot &>(a_Entity).IsSitting())
			);
		}
	);
}





bool cOcelot::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (a_TDI.DamageType == dtFalling)
	{
		return false;
	}

	return Super::DoTakeDamage(a_TDI);
}
