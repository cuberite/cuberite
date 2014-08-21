#pragma once

#include "NewMonster.h"





class cNewZombie :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewZombie(bool a_IsVillagerZombie);

	CLASS_PROTODEF(cNewZombie)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	
	bool IsVillagerZombie(void) const { return m_IsVillagerZombie; }
	bool IsConverting    (void) const { return m_IsConverting; }

private:

	bool m_IsVillagerZombie;
	bool m_IsConverting;

} ;




