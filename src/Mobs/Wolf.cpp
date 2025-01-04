
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wolf.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Items/ItemHandler.h"
#include "../Items/ItemSpawnEgg.h"





cWolf::cWolf(void) :
	Super("Wolf", mtWolf, "entity.wolf.hurt", "entity.wolf.death", "entity.wolf.ambient", 0.6f, 0.85f),
	m_IsSitting(false),
	m_IsTame(false),
	m_IsBegging(false),
	m_IsAngry(false),
	m_CollarColor(E_META_DYE_ORANGE),
	m_NotificationCooldown(0)
{
}





bool cWolf::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	cPawn * PreviousTarget = GetTarget();
	if (!Super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPawn())
	{
		auto currTarget = GetTarget();
		if ((currTarget != nullptr) && currTarget->IsPlayer())
		{
			if (m_IsTame)
			{
				if ((static_cast<cPlayer*>(currTarget)->GetUUID() == m_OwnerUUID))
				{
					SetTarget(PreviousTarget);  // Do not attack owner
				}
				else
				{
					SetIsSitting(false);
					NotifyAlliesOfFight(static_cast<cPawn*>(a_TDI.Attacker));
				}
			}
			else
			{
				m_IsAngry = true;
			}
		}
		else if (m_IsTame)
		{
			SetIsSitting(false);
			NotifyAlliesOfFight(static_cast<cPawn*>(a_TDI.Attacker));
		}
	}

	m_World->BroadcastEntityMetadata(*this);  // Broadcast health and possibly angry face
	return true;
}





void cWolf::NotifyAlliesOfFight(cPawn * a_Opponent)
{
	if (GetOwnerName() == "")
	{
		return;
	}
	m_NotificationCooldown = 15;

	m_World->DoWithPlayerByUUID(m_OwnerUUID, [=](cPlayer & a_Player)
		{
			a_Player.NotifyNearbyWolves(a_Opponent, false);
			return false;
		}
	);
}





bool cWolf::Attack(std::chrono::milliseconds a_Dt)
{
	UNUSED(a_Dt);

	if ((GetTarget() != nullptr) && (GetTarget()->IsPlayer()))
	{
		if (static_cast<cPlayer *>(GetTarget())->GetUUID() == m_OwnerUUID)
		{
			SetTarget(nullptr);
			return false;
		}
	}

	NotifyAlliesOfFight(static_cast<cPawn*>(GetTarget()));
	return Super::Attack(a_Dt);

}





void cWolf::ReceiveNearbyFightInfo(const cUUID & a_PlayerID, cPawn * a_Opponent, bool a_IsPlayerInvolved)
{
	if (
		(a_Opponent == nullptr) || IsSitting() || (!IsTame()) ||
		(!a_Opponent->IsPawn()) || (a_PlayerID != m_OwnerUUID)
	)
	{
		return;
	}

	// If we already have a target
	if (GetTarget() != nullptr)
	{
		// If a wolf is asking for help and we already have a target, do nothing
		if (!a_IsPlayerInvolved)
		{
			return;
		}
		// If a player is asking for help and we already have a target,
		// there's a 50% chance of helping and a 50% chance of doing nothing
		// This helps spread a wolf pack's targets over several mobs
		else if (GetRandomProvider().RandBool())
		{
			return;
		}
	}

	if (a_Opponent->IsPlayer() && static_cast<cPlayer *>(a_Opponent)->GetUUID() == m_OwnerUUID)
	{
		return;  // Our owner has hurt himself, avoid attacking them.
	}

	if (a_Opponent->IsMob() && static_cast<cMonster *>(a_Opponent)->GetMobType() == mtWolf)
	{
		cWolf * Wolf = static_cast<cWolf *>(a_Opponent);
		if (Wolf->GetOwnerUUID() == GetOwnerUUID())
		{
			return;  // Our owner attacked one of their wolves. Abort attacking wolf.
		}
	}

	SetTarget(a_Opponent);


}





void cWolf::OnRightClicked(cPlayer & a_Player)
{
	cMonster::OnRightClicked(a_Player);
	const cItem & EquippedItem = a_Player.GetEquippedItem();
	const auto EquippedItemType = EquippedItem.m_ItemType;

	if (!IsTame() && !IsAngry())
	{
		// If the player is holding a bone, try to tame the wolf:
		if (EquippedItemType == Item::Bone)
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
			}

			if (GetRandomProvider().RandBool(0.125))
			{
				// Taming succeeded
				SetMaxHealth(20);
				SetIsTame(true);
				SetOwner(a_Player.GetName(), a_Player.GetUUID());
				m_World->BroadcastEntityAnimation(*this, EntityAnimation::WolfTamingSucceeds);
			}
			else
			{
				// Taming failed
				m_World->BroadcastEntityAnimation(*this, EntityAnimation::WolfTamingFails);
			}
		}
	}
	else if (IsTame())
	{
		if (a_Player.GetUUID() == m_OwnerUUID)
		{
			cMonster::RightClickFeed(a_Player);
		}
		// Feed the wolf, restoring its health, or dye its collar:
		switch (EquippedItemType)
		{
			case Item::Beef:
			case Item::CookedBeef:
			case Item::Porkchop:
			case Item::CookedPorkchop:
			case Item::Chicken:
			case Item::CookedChicken:
			case Item::RottenFlesh:
			case Item::Mutton:
			case Item::CookedMutton:
			case Item::Rabbit:
			case Item::CookedRabbit:
			{
				if (m_Health < m_MaxHealth)
				{
					Heal(EquippedItem.GetHandler().GetFoodInfo(&EquippedItem).FoodLevel);
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.GetInventory().RemoveOneEquippedItem();
					}
				}
				else if (a_Player.GetUUID() == m_OwnerUUID)  // Is the player the owner of the dog?
				{
					if (IsBaby())
					{
						m_AgingTimer = FloorC(m_AgingTimer * 0.9);
					}
				}
				break;
			}
			case Item::BlackDye:
			case Item::BlueDye:
			case Item::BrownDye:
			case Item::CyanDye:
			case Item::GrayDye:
			case Item::GreenDye:
			case Item::LightBlueDye:
			case Item::LightGrayDye:
			case Item::LimeDye:
			case Item::MagentaDye:
			case Item::OrangeDye:
			case Item::PinkDye:
			case Item::PurpleDye:
			case Item::RedDye:
			case Item::WhiteDye:
			case Item::YellowDye:
			{
				if (a_Player.GetUUID() == m_OwnerUUID)  // Is the player the owner of the dog?
				{
					SetCollarColor(EquippedItem.m_ItemDamage);
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.GetInventory().RemoveOneEquippedItem();
					}
				}
				break;
			}
			// Multiplication is handled in cMonster. Just prevents from sitting down.
			case Item::BatSpawnEgg:
			case Item::BlazeSpawnEgg:
			case Item::CaveSpiderSpawnEgg:
			case Item::ChickenSpawnEgg:
			case Item::CowSpawnEgg:
			case Item::CreeperSpawnEgg:
			case Item::EndermanSpawnEgg:
			case Item::GhastSpawnEgg:
			case Item::GuardianSpawnEgg:
			case Item::HorseSpawnEgg:
			case Item::MagmaCubeSpawnEgg:
			case Item::MooshroomSpawnEgg:
			case Item::OcelotSpawnEgg:
			case Item::PigSpawnEgg:
			case Item::RabbitSpawnEgg:
			case Item::SheepSpawnEgg:
			case Item::SilverfishSpawnEgg:
			case Item::SkeletonSpawnEgg:
			case Item::SlimeSpawnEgg:
			case Item::SpiderSpawnEgg:
			case Item::SquidSpawnEgg:
			case Item::VillagerSpawnEgg:
			case Item::WitchSpawnEgg:
			case Item::WitherSkeletonSpawnEgg:
			case Item::WolfSpawnEgg:
			case Item::ZombieSpawnEgg:
			case Item::ZombiePigmanSpawnEgg:
			case Item::ZombieVillagerSpawnEgg:
			{
				break;
			}
			default:
			{
				if (a_Player.GetUUID() == m_OwnerUUID)  // Is the player the owner of the dog?
				{
					SetIsSitting(!IsSitting());
				}
			}
		}
	}

	if (!IsTame())
	{
		switch (EquippedItemType)
		{
			case Item::BatSpawnEgg:
			case Item::BlazeSpawnEgg:
			case Item::CaveSpiderSpawnEgg:
			case Item::ChickenSpawnEgg:
			case Item::CowSpawnEgg:
			case Item::CreeperSpawnEgg:
			case Item::EndermanSpawnEgg:
			case Item::GhastSpawnEgg:
			case Item::GuardianSpawnEgg:
			case Item::HorseSpawnEgg:
			case Item::MagmaCubeSpawnEgg:
			case Item::MooshroomSpawnEgg:
			case Item::OcelotSpawnEgg:
			case Item::PigSpawnEgg:
			case Item::RabbitSpawnEgg:
			case Item::SheepSpawnEgg:
			case Item::SilverfishSpawnEgg:
			case Item::SkeletonSpawnEgg:
			case Item::SlimeSpawnEgg:
			case Item::SpiderSpawnEgg:
			case Item::SquidSpawnEgg:
			case Item::VillagerSpawnEgg:
			case Item::WitchSpawnEgg:
			case Item::WitherSkeletonSpawnEgg:
			case Item::WolfSpawnEgg:
			case Item::ZombieSpawnEgg:
			case Item::ZombiePigmanSpawnEgg:
			case Item::ZombieVillagerSpawnEgg:
			{
				eMonsterType MonsterType = cItemSpawnEggHandler::ItemToMonsterType(EquippedItem.m_ItemType);
				if (
					(MonsterType == m_MobType) &&
					(m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), m_MobType, true) != cEntity::INVALID_ID))  // Spawning succeeded
				{
					if (!a_Player.IsGameModeCreative())
					{
						// The mob was spawned, "use" the item:
						a_Player.GetInventory().RemoveOneEquippedItem();
					}
				}
				break;
			}
			default:
			{
				break;
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
		if (m_NotificationCooldown > 0)
		{
			m_NotificationCooldown -= 1;
		}
	}
	else
	{
		Super::Tick(a_Dt, a_Chunk);
	}

	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (GetTarget() == nullptr)
	{
		m_World->DoWithNearestPlayer(GetPosition(), static_cast<float>(m_SightDistance), [&](cPlayer & a_Player) -> bool
		{
			switch (a_Player.GetEquippedItem().m_ItemType)
			{
				case Item::Bone:
				case Item::Beef:
				case Item::CookedBeef:
				case Item::Chicken:
				case Item::CookedChicken:
				case Item::RottenFlesh:
				case Item::Porkchop:
				case Item::CookedPorkchop:
				{
					if (!IsBegging())
					{
						SetIsBegging(true);
						m_World->BroadcastEntityMetadata(*this);
					}

					m_FinalDestination = a_Player.GetPosition();  // So that we will look at a player holding food

					// Don't move to the player if the wolf is sitting.
					if (!IsSitting())
					{
						MoveToPosition(a_Player.GetPosition());
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

			return true;
		});
	}
	else
	{
		if (IsSitting())
		{
			SetTarget(nullptr);
		}
		else
		{
			MoveToPosition(GetTarget()->GetPosition());
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

	cMonster::LoveTick();
}





void cWolf::TickFollowPlayer()
{
	Vector3d OwnerPos;
	bool OwnerFlying;
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
		if (Distance > 20)
		{
			if (!OwnerFlying)
			{
				OwnerPos.y = FindFirstNonAirBlockPosition(OwnerPos.x, OwnerPos.z);
				TeleportToCoords(OwnerPos.x, OwnerPos.y, OwnerPos.z);
				SetTarget(nullptr);
			}
		}
		if (Distance < 2)
		{
			if (GetTarget() == nullptr)
			{
				StopMovingToPosition();
			}
		}
		else
		{
			if (GetTarget() == nullptr)
			{
				if (!OwnerFlying)
				{
					MoveToPosition(OwnerPos);
				}
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





void cWolf::InheritFromParents(cMonster * a_Parent1, cMonster * a_Parent2)
{
	const auto Parent1 = static_cast<cWolf *>(a_Parent1);
	const auto Parent2 = static_cast<cWolf *>(a_Parent2);
	if (Parent1->GetOwnerUUID() == Parent2->GetOwnerUUID())
	{
		SetOwner(Parent1->GetOwnerName(), Parent2->GetOwnerUUID());
	}
	else
	{
		auto Parent1Age = Parent1->GetAge();
		auto Parent2Age = Parent2->GetAge();

		if (Parent1Age > Parent2Age)
		{
			SetOwner(Parent2->GetOwnerName(), Parent2->GetOwnerUUID());
		}
		else
		{
			SetOwner(Parent1->GetOwnerName(), Parent1->GetOwnerUUID());
		}
	}
}
