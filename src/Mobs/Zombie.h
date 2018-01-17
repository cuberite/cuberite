#pragma once

#include "AggressiveMonster.h"





class cZombie :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;

public:
	cZombie(bool a_IsVillagerZombie);

	CLASS_PROTODEF(cZombie)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool IsUndead(void) override { return true; }

	bool IsVillagerZombie(void) const { return m_IsVillagerZombie; }
	bool IsConverting    (void) const { return m_IsConverting; }

private:

	bool m_IsVillagerZombie;
	bool m_IsConverting;

} ;




