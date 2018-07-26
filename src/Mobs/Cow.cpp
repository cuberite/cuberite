
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cow.h"
#include "../Entities/Player.h"





cCow::cCow(void) :
	super("Cow", mtCow, "entity.cow.hurt", "entity.cow.death", 0.9, 1.3),
	m_Blackboard(*this)
{
	using namespace BehaviorTree;
	using std::chrono::seconds;

	auto Wander = cpp14::make_unique<cSequence>(
		MakeVector<std::unique_ptr<cTask>>(
			cpp14::make_unique<cRandomPosition>("WanderPos", 10.0),
			cpp14::make_unique<cSucceeder>(cpp14::make_unique<cMoveToPosition>("WanderPos")),
			cpp14::make_unique<cRandomWait>(seconds(1), seconds(4))
		)
	);

	auto Panic = cpp14::make_unique<cSequence>(
		MakeVector<std::unique_ptr<cTask>>(
			cpp14::make_unique<cRandomPosition>("PanicPos", 20.0),
			cpp14::make_unique<cMoveToPosition>("PanicPos")
		)
	);

	m_BehaviorTree = cpp14::make_unique<cDynamicGuardSelector>(
		MakeVector<std::unique_ptr<cGuardedTask>>(
			cpp14::make_unique<cGuardedTask>(
				cpp14::make_unique<cHealthRange>(0.0f, 0.5f * m_MaxHealth),
				std::move(Panic)
			),
			cpp14::make_unique<cGuardedTask>(
				nullptr,
				std::move(Wander)
			)
		)
	);
}





void cCow::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (IsBaby())
	{
		return;  // Babies don't drop items
	}

	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_LEATHER);
	AddRandomDropItem(a_Drops, 1, 3 + LootingLevel, IsOnFire() ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
}





void cCow::OnRightClicked(cPlayer & a_Player)
{
	super::OnRightClicked(a_Player);

	short HeldItem = a_Player.GetEquippedItem().m_ItemType;
	if (HeldItem == E_ITEM_BUCKET)
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
			a_Player.GetInventory().AddItem(E_ITEM_MILK);
		}
	}
}





void cCow::InStateIdle(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	m_BehaviorTree->Tick(m_Blackboard);
}
