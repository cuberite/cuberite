
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Blaze.h"
#include "../World.h"
#include "../Entities/FireChargeEntity.h"


void FireballShootingFunction(cBehaviorAttackerRanged & a_Behavior,
	cMonster & a_Attacker, cPawn & a_Attacked)
{
	UNUSED(a_Behavior);

	// Setting this higher gives us more wiggle room for attackrate
	Vector3d Speed = a_Attacker.GetLookVector() * 20;
	Speed.y = Speed.y + 3;

	auto FireCharge = cpp14::make_unique<cFireChargeEntity>(&a_Attacker,
		a_Attacker.GetPosX(), a_Attacker.GetPosY() + 1, a_Attacker.GetPosZ(), Speed);
	auto FireChargePtr = FireCharge.get();
	FireChargePtr->Initialize(std::move(FireCharge), *(a_Attacker.GetWorld()));
}


cBlaze::cBlaze(void) :
	super(mtBlaze, "entity.blaze.hurt", "entity.blaze.death", 0.6, 1.8),
	m_BehaviorAttackerRanged(FireballShootingFunction, 3, 15)
{
	m_EMPersonality = AGGRESSIVE;
	m_BehaviorAttackerRanged.AttachToMonster(*this);
	m_BehaviorDoNothing.AttachToMonster(*this);
	m_BehaviorAggressive.AttachToMonster(*this);
	SetGravity(-8.0f);
	SetAirDrag(0.05f);
	GetMonsterConfig("Blaze");
}





void cBlaze::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if ((a_Killer != nullptr) && (a_Killer->IsPlayer() || a_Killer->IsA("cWolf")))
	{
		unsigned int LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
		AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_BLAZE_ROD);
	}
}
