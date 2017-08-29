
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pig.h"
#include "../Entities/Player.h"
#include "../World.h"





cPig::cPig(void) :
	super("Pig", mtPig, "entity.pig.hurt", "entity.pig.death", 0.9, 0.9),
	m_bIsSaddled(false)
{
	m_EMPersonality = PASSIVE;
	m_BehaviorBreeder.AttachToMonster(*this);
	m_BehaviorCoward.AttachToMonster(*this);
	m_BehaviorItemFollower.AttachToMonster(*this);
	m_BehaviorWanderer.AttachToMonster(*this);
}





void cPig::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 1, 3 + LootingLevel, IsOnFire() ? E_ITEM_COOKED_PORKCHOP : E_ITEM_RAW_PORKCHOP);
	if (m_bIsSaddled)
	{
		a_Drops.push_back(cItem(E_ITEM_SADDLE, 1));
	}
}





void cPig::OnRightClicked(cPlayer & a_Player)
{
	super::OnRightClicked(a_Player);

	// Behavior note: saddling is pig-specific. It is not transferrable to other mobs.
	// Therefore saddling is not a standalone behavior and is hardcoded into the pig.

	if (m_bIsSaddled)
	{
		if (m_Attachee != nullptr)
		{
			if (m_Attachee->GetUniqueID() == a_Player.GetUniqueID())
			{
				// This player is already sitting in, they want out.
				a_Player.Detach();
				return;
			}

			if (m_Attachee->IsPlayer())
			{
				// Another player is already sitting in here, cannot attach
				return;
			}

			// Detach whatever is sitting in this pig now:
			m_Attachee->Detach();
		}

		// Attach the player to this pig
		a_Player.AttachTo(this);
	}
	else if (a_Player.GetEquippedItem().m_ItemType == E_ITEM_SADDLE)
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}

		// Set saddle state & broadcast metadata
		m_bIsSaddled = true;
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cPig::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	// Behavior note: saddling is pig-specific. It is not transferrable to other mobs.
	// Therefore saddling is not a standalone behavior and is hardcoded into the pig.

	// If the attachee player is holding a carrot-on-stick, let them drive this pig:
	if (m_bIsSaddled && (m_Attachee != nullptr))
	{
		if (m_Attachee->IsPlayer() && (m_Attachee->GetEquippedWeapon().m_ItemType == E_ITEM_CARROT_ON_STICK))
		{
			MoveToPosition((m_Attachee->GetPosition()) + (m_Attachee->GetLookVector()*10));
		}
	}
}





bool cPig::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if (a_TDI.DamageType == dtLightning)
	{
		Destroy();
		m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), mtZombiePigman, false);
		return true;
	}
	return true;
}





cBehaviorBreeder * cPig::GetBehaviorBreeder()
{
	return &m_BehaviorBreeder;
}





const cBehaviorBreeder * cPig::GetBehaviorBreeder() const
{
	return static_cast<const cBehaviorBreeder *>(&m_BehaviorBreeder);
}



