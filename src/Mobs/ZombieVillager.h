#pragma once

#include "AggressiveMonster.h"
#include "Villager.h"





class cZombieVillager:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	cZombieVillager(cVillager::eVillagerType a_Profession);

	CLASS_PROTODEF(cZombieVillager)

	virtual void GetDrops       (cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void Tick           (std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked (cPlayer & a_Player) override;
	virtual bool IsUndead       (void) override { return true; }

	int ConversionTime                     (void) const { return m_ConversionTime; }
	cVillager::eVillagerType GetProfession (void) const { return m_Profession; }

private:

	int m_ConversionTime;
	cVillager::eVillagerType m_Profession;

} ;
