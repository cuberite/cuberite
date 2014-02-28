
#pragma once

#include "AggressiveMonster.h"





class cCreeper :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cCreeper(void);

	CLASS_PROTODEF(cCreeper);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Attack(float a_Dt) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	bool IsBlowing(void) const {return m_bIsBlowing; }
	bool IsCharged(void) const {return m_bIsCharged; }

private:

	bool m_bIsBlowing, m_bIsCharged;
	int m_ExplodingTimer;

} ;




