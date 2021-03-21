
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cow.h"
#include "../Entities/Player.h"





cCow::cCow(void) :
	Super("Cow", mtCow, "entity.cow.hurt", "entity.cow.death", "entity.cow.ambient", 0.9, 1.3),
	m_Blackboard(*this)
{
	using namespace BehaviorTree;
	using std::chrono::seconds;

	auto Wander = std::make_unique<cSequence>(
		MakeVector<std::unique_ptr<cTask>>(
			std::make_unique<cRandomPosition>("WanderPos", 10.0),
			std::make_unique<cSucceeder>(std::make_unique<cMoveToPosition>("WanderPos")),
			std::make_unique<cRandomWait>(seconds(1), seconds(4))
		)
	);

	auto Panic = std::make_unique<cSequence>(
		MakeVector<std::unique_ptr<cTask>>(
			std::make_unique<cRandomPosition>("PanicPos", 20.0),
			std::make_unique<cMoveToPosition>("PanicPos")
		)
	);

	m_BehaviorTree = std::make_unique<cDynamicGuardSelector>(
		MakeVector<std::unique_ptr<cGuardedTask>>(
			std::make_unique<cGuardedTask>(
				std::make_unique<cHealthRange>(0.0f, 0.5f * m_MaxHealth),
				std::move(Panic)
			),
			std::make_unique<cGuardedTask>(
				nullptr,
				std::move(Wander)
			)
		)
	);

	m_BehaviorTree = BehaviorTree::CreateBehaviourTree(eMonsterType::mtCow);
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
	Super::OnRightClicked(a_Player);

	short HeldItem = a_Player.GetEquippedItem().m_ItemType;
	if (HeldItem == E_ITEM_BUCKET)
	{
		// Milk the cow.
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.ReplaceOneEquippedItemTossRest(cItem(E_ITEM_MILK));
		}
	}
}





void cCow::InStateIdle(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (m_BehaviorTree)
	{
		m_BehaviorTree->Tick(m_Blackboard);
	}
}
